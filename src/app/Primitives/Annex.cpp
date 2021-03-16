#include "pch.hpp"
#include "Annex.hpp"


Annex::Annex(FT_Face font, uint32_t font_size) {
    characterSet.reserve(128);

    lwvl::Framebuffer frame;
    m_atlas.bind();
    m_atlas.construct(16 * font_size, 8 * font_size, nullptr);
    m_atlas.filter(lwvl::Filter::Nearest);

    frame.bind();
    frame.attach(lwvl::Attachment::Color, m_atlas);

    lwvl::VertexArray vao;
    lwvl::ArrayBuffer vbo;
    vao.bind();
    vbo.bind();
    vbo.usage(lwvl::Usage::Static);

    std::array<float, 8> shape {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };
    vbo.construct(shape.begin(), shape.end());
    vao.attribute(2, GL_FLOAT, 2 * sizeof(float), 0);

    lwvl::ElementBuffer ebo;
    ebo.bind();
    ebo.usage(lwvl::Usage::Static);

    std::array<uint32_t, 6> atlasIndices {
        0, 1, 2,
        2, 3, 0
    };
    ebo.construct(atlasIndices.begin(), atlasIndices.end());

    m_vao.bind();
    m_vbo.bind();
    vbo.usage(lwvl::Usage::Static);

//    std::array<float, 16> atlas;
}

void Annex::draw() {

}
