#pragma once

#include "pch.hpp"
#include "VertexArray.hpp"
#include "Buffer.hpp"
#include "Framebuffer.hpp"
#include "Texture.hpp"
#include "Shader.hpp"


struct Glyph {
    float texCoordL, texCoordB, texCoordR, texCoordT;
    float width, height;
    float xBearing, yBearing;
    float advance;

public:
    Glyph(float left, float bottom, float right, float top, float w, float h, float xBear, float yBear, float adv);
};


class Font : public lwvl::Texture2D {
    std::vector<Glyph> characterSet;

protected:
    // Disallow the user constructing over the font atlas.
    using lwvl::Texture2D::construct;

public:
    Font(const char *fontFile, uint32_t fontSize);

    const Glyph &glyph(unsigned char val);
};
