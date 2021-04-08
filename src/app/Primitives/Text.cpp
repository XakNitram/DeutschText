#include "pch.hpp"
#include "Text.hpp"

Text::Text(Font &font, std::string initialText, float x, float y, float initialScale, lwvl::ShaderProgram &textShader) :
    program(textShader), text(std::move(initialText)), font(font), xOffset(x), yOffset(y), textScale(initialScale) {
    program.bind();
    u_TextScale.set1f(initialScale);
    u_TextOffset.set2f(x, y);

    bufferArray.bind();
    vertexBuffer.bind();
    std::array<float, 8> quadData{
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };

    vertexBuffer.construct(quadData.begin(), quadData.end());
    bufferArray.attribute(2, GL_FLOAT, 2 * sizeof(float), 0);

    // Set Buffer Texture Shader Uniform
    coordTexture.slot(1);
    program.uniform("coordinates").set1i(1);

    // Construct the Text
    constructText();

    bufferArray.bind();
    modelBuffer.bind();
    bufferArray.attribute(2, GL_FLOAT, 4 * sizeof(float), 0, 1);
    bufferArray.attribute(2, GL_FLOAT, 4 * sizeof(float), 2 * sizeof(float), 1);

    lwvl::VertexArray::clear();
    lwvl::ArrayBuffer::clear();
    lwvl::TextureBuffer::clear();
}

void Text::constructText() {
    bufferArray.bind();

    const size_t stringLength = text.length();
    bufferArray.instances(stringLength);

    // Different types of buffers so they can be bound together.
    modelBuffer.bind();
    auto *coordData = new float[8 * stringLength];

    coordBuffer.bind();
    auto *modelData = new float[4 * stringLength];

    std::vector<Glyph> letterCache;
    letterCache.reserve(stringLength);
    letterCache.push_back(font.get().glyph(text[0]));

    float stringWidth;
    float stringHeight;
    if (stringLength > 1) {
        stringWidth = letterCache[0].advance - letterCache[0].xBearing;
        stringHeight = 0.0f;

        for (size_t i = 1; i < stringLength; i++) {
            letterCache.push_back(font.get().glyph(text[i]));
            Glyph &letter = letterCache[i];

            stringWidth += letter.advance;

            const float letterHeight = letter.yBearing;
            if (letterHeight > stringHeight) {
                stringHeight = letterHeight;
            }
        }

        // Fix the width for the last character
        stringWidth += letterCache[stringLength - 1].width - letterCache[stringLength - 1].advance;
    } else {
        stringWidth = letterCache[0].width;
        stringHeight = letterCache[0].height;
    }

    textWidth = stringWidth;
    textHeight = stringHeight;

    float advance = 0.0f;

    for (size_t i = 0; i < stringLength; i++) {
        Glyph &letter = letterCache[i];

        const float modelInstance[4]{
            // Scale
            letter.width,
            letter.height,

            // Offset
            0.0f + (letter.xBearing + advance),
            0.0f - (letter.height - letter.yBearing)
        };
        memcpy(modelData + (i * 4), modelInstance, sizeof(modelInstance));

        advance += letter.advance;

        const float coordInstance[8]{
            letter.texCoordL, letter.texCoordB,
            letter.texCoordR, letter.texCoordB,
            letter.texCoordR, letter.texCoordT,
            letter.texCoordL, letter.texCoordT
        };
        memcpy(coordData + (i * 8), coordInstance, sizeof(coordInstance));
    }

    modelBuffer.construct(modelData, 4 * stringLength);

    // Construct TexCoord Buffer
    coordBuffer.construct(coordData, 8 * stringLength);

    // Attach TexCoord Buffer to Buffer Texture
    coordTexture.bind();
    coordTexture.construct(coordBuffer, lwvl::ChannelLayout::RG32F);

    delete[] modelData;
    delete[] coordData;
}

float Text::Width() const {
    return textWidth;
}

float Text::Height() const {
    return textHeight;
}

float Text::Scale() const {
    return textScale;
}

void Text::Scale(float value) {
    textScale = value;
}

void Text::update(float x, float y) {
    xOffset = x;
    yOffset = y;
}

void Text::draw() {
    program.bind();
    u_TextScale.set1f(textScale);
    u_TextOffset.set2f(xOffset, yOffset);
    font.get().bind();
    coordTexture.bind();
    bufferArray.bind();
    bufferArray.drawArrays(lwvl::PrimitiveMode::TriangleFan, 4);
}


// TextFactory
TextFactory::TextFactory(Font &font) : font(font) {
    lwvl::VertexShader vs{lwvl::VertexShader::readFile("Data/Shaders/text.vert")};
    lwvl::FragmentShader fs{lwvl::FragmentShader::readFile("Data/Shaders/text.frag")};
    //lwvl::FragmentShader fs{lwvl::FragmentShader::readFile("Data/Shaders/spacetime.frag")};

    program.link(vs, fs);
    program.bind();
    program.uniform("atlas").set1i(font.slot());
}

Text TextFactory::create(std::string &text, float x, float y, float scale) {
    return Text(font, text, x, y, scale, program);
}

Text TextFactory::create(const char *text, float x, float y, float scale) {
    return Text(font, {text}, x, y, scale, program);
}


