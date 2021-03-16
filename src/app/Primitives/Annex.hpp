#pragma once
#include "pch.hpp"
#include "VertexArray.hpp"
#include "Buffer.hpp"
#include "Framebuffer.hpp"
#include "Texture.hpp"
#include "Shader.hpp"


class Annex {
    struct Glyph {
        float texCoordX, texCoordY;
        float width, height;
        float xBearing, yBearing;
        float advance;
    };

    std::vector<Glyph> characterSet;
    lwvl::VertexArray m_vao;
    lwvl::ArrayBuffer m_vbo;
    lwvl::ShaderProgram m_shader;
    lwvl::Texture m_atlas;

public:
    explicit Annex(FT_Face font, uint32_t font_size);

    void draw();
};
