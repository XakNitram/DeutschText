#include "pch.hpp"
#include "Font.hpp"


Glyph::Glyph(
    float left, float bottom, float right, float top,
    float w, float h, float xBear, float yBear, float adv
) :
    texCoordL(left), texCoordB(bottom),
    texCoordR(right), texCoordT(top),
    width(w), height(h),
    xBearing(xBear), yBearing(yBear),
    advance(adv) {}


Font::Font(const char *fontFile, uint32_t fontSize) : pixelScale(1.0f / static_cast<float>(fontSize)) {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "Could not initialize FreeType Library." << std::endl;
        throw std::exception();
    }

    FT_Face font;
    if (FT_New_Face(ft, fontFile, 0, &font)) {
        std::cerr << "Failed to load font from Data/Fonts/Deutsch.ttf." << std::endl;
        throw std::exception();
    }

    FT_Set_Pixel_Sizes(font, 0, fontSize);

    characterSet.reserve(128);

    const uint32_t atlasWidth = 16 * fontSize;
    const uint32_t atlasHeight = 8 * fontSize;

    const auto cellSize = static_cast<float>(fontSize);
    const auto decWidth = static_cast<float>(atlasWidth);
    const auto decHeight = static_cast<float>(atlasHeight);

    bind();
    construct(
        atlasWidth, atlasHeight, nullptr,
        lwvl::ChannelLayout::RGBA,
        lwvl::ChannelOrder::RGBA,
        lwvl::ByteFormat::Byte
    );
    filter(lwvl::Filter::Linear);

    lwvl::Framebuffer frame;
    frame.bind();
    frame.attach(lwvl::Attachment::Color, *this);

    lwvl::ShaderProgram atlasPipeline;
    lwvl::VertexShader vs(lwvl::VertexShader::readFile("Data/Shaders/glyph.vert"));
    //lwvl::FragmentShader fs(lwvl::FragmentShader::readFile("Data/Shaders/mazing.frag"));
    lwvl::FragmentShader fs(lwvl::FragmentShader::readFile("Data/Shaders/glyph.frag"));

    atlasPipeline.link(vs, fs);
    atlasPipeline.bind();

    lwvl::Uniform glyphOffset = atlasPipeline.uniform("offset");
    lwvl::Uniform glyphScale = atlasPipeline.uniform("scale");
    //lwvl::Uniform glyphResolution = atlasPipeline.uniform("resolution");

    atlasPipeline.uniform("projection").set2DOrthographic(
        decHeight, 0.0f, decWidth, 0.0f
    );

    lwvl::Texture2D glyphTexture;

    glyphTexture.slot(1);
    glyphTexture.bind();
    glyphTexture.filter(lwvl::Filter::Linear);

    atlasPipeline.uniform("glyph").set1i(1);

    lwvl::VertexArray vao;
    lwvl::ArrayBuffer vbo;
    vao.bind();
    vbo.bind();
    vbo.usage(lwvl::Usage::Static);

    std::array<float, 16> glyphQuadData{
        0.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f
    };

    vbo.construct(glyphQuadData.begin(), glyphQuadData.end());
    vao.attribute(2, GL_FLOAT, 4 * sizeof(float), 0);
    vao.attribute(2, GL_FLOAT, 4 * sizeof(float), 2 * sizeof(float));

    const float widthPixel = 1.0f / decWidth;
    const float heightPixel = 1.0f / decHeight;

    // Viewport is not bound to the framebuffer so we have to restore the previous viewport.
    GLsizei prevViewport[4];
    glGetIntegerv(GL_VIEWPORT, prevViewport);
    const auto[prevX, prevY, prevWidth, prevHeight] = prevViewport;

    glViewport(0, 0, atlasWidth, atlasHeight);

    GLint previousUnpackAlignment;
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &previousUnpackAlignment);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(font, c, FT_LOAD_RENDER)) {
            std::exception("Failed to load a glyph.");
        }

        // Reconstruct the texture for each glyph since they'll be different sizes in memory.
        glyphTexture.construct(
            font->glyph->bitmap.width,
            font->glyph->bitmap.rows,
            font->glyph->bitmap.buffer,
            lwvl::ChannelLayout::Red,
            lwvl::ChannelOrder::Red,
            lwvl::ByteFormat::UnsignedByte
        );


        const auto charWidth = static_cast<float>(font->glyph->bitmap.width);
        const auto charHeight = static_cast<float>(font->glyph->bitmap.rows);

        glyphScale.set2f(charWidth, charHeight);
        //glyphResolution.set2f(cellSize, cellSize);

        const auto[row, col] = std::div(static_cast<uint32_t>(c), 16);

        const float texLeft = (cellSize - charWidth) * 0.5f + static_cast<float>(col) * cellSize;
        const float texBottom = (cellSize - charHeight) * 0.5f + static_cast<float>(8 - (row + 1)) * cellSize;
        const float texRight = texLeft + charWidth;
        const float texTop = texBottom + charHeight;

        glyphOffset.set2f(texLeft, texBottom);

        characterSet.emplace_back(
            texLeft * widthPixel, texBottom * heightPixel,
            texRight * widthPixel, texTop * heightPixel,
            charWidth, charHeight,

            static_cast<float>(font->glyph->bitmap_left),
            static_cast<float>(font->glyph->bitmap_top),

            // Divide by 64 to get value in pixels. (Just the way FreeType works.)
            static_cast<float>(font->glyph->advance.x >> 6)
        );

        vao.drawArrays(lwvl::PrimitiveMode::TriangleFan, 4);
    }

    glViewport(prevX, prevY, prevWidth, prevHeight);
    lwvl::Framebuffer::clear();
    glPixelStorei(GL_UNPACK_ALIGNMENT, previousUnpackAlignment);

    FT_Done_Face(font);
    FT_Done_FreeType(ft);
}

const Glyph &Font::glyph(unsigned char val) {
    return characterSet[static_cast<size_t>(val)];
}

float Font::scaler() const {
    return pixelScale;
}
