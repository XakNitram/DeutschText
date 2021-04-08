#pragma once

#include "pch.hpp"
#include "Primitives/Font.hpp"


class Text {
    lwvl::ShaderProgram program;
    lwvl::Uniform u_TextScale = program.uniform("stringScale");
    lwvl::Uniform u_TextOffset = program.uniform("stringOffset");

    lwvl::VertexArray bufferArray;
    lwvl::ArrayBuffer vertexBuffer{lwvl::Usage::Static};
    lwvl::ArrayBuffer modelBuffer{lwvl::Usage::Dynamic};
    lwvl::TextureBuffer coordBuffer{lwvl::Usage::Dynamic};
    lwvl::BufferTexture coordTexture;

    std::string text;

    std::reference_wrapper<Font> font;

    float xOffset, yOffset;
    float textScale;
    float textWidth = 0.0f;
    float textHeight = 0.0f;

    void constructText();

public:
    explicit Text(
        Font &font, std::string initialText, float x, float y, float initialScale, lwvl::ShaderProgram &textShader
    );

    [[nodiscard]] float Width() const;

    [[nodiscard]] float Height() const;

    [[nodiscard]] float Scale() const;

    void Scale(float value);

    void update(float x, float y);

    void draw();
};


class TextFactory {
    std::reference_wrapper<Font> font;

public:
    lwvl::ShaderProgram program;

    explicit TextFactory(Font &font);

    Text create(std::string &text, float x, float y, float scale);

    Text create(const char *text, float x, float y, float scale);
};
