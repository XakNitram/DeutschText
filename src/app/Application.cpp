#include "pch.hpp"
#include "Core/Window.hpp"
#include "Primitives/Font.hpp"
#include "GLDebug.hpp"


class Application {
    Window m_window;

    static void debug_gl(
        lwvl::debug::Source source, lwvl::debug::Type type,
        lwvl::debug::Severity severity, unsigned int id, int length,
        const char *message, const void *userState
    ) {
        std::cout << "[OpenGL] " << message << std::endl;
    }

public:
    Application(uint32_t width, uint32_t height) : m_window(width, height, "DeutschText") {}

    int run() {
        //const uint32_t pixelFontSize = 64;
        const uint32_t pixelFontSize = 384;

        lwvl::debug::GLEventListener listener(debug_gl);

        Font deutsch("Data/Fonts/Deutsch.ttf", pixelFontSize);

        lwvl::ShaderProgram atlasPipeline;
        {
            lwvl::VertexShader   vs(lwvl::VertexShader::readFile("Data/Shaders/atlas.vert"));
            lwvl::FragmentShader fs(lwvl::FragmentShader::readFile("Data/Shaders/atlas.frag"));
            //lwvl::FragmentShader fs(lwvl::FragmentShader::readFile("Data/Shaders/mazing.frag"));
            atlasPipeline.link(vs, fs);
        }
        atlasPipeline.bind();

        const auto width  = static_cast<float>(m_window.config.width);
        const auto height = static_cast<float>(m_window.config.height);
        atlasPipeline.uniform("projection").set2DOrthographic(
            height, 0.0f,
            width, 0.0f
        );

        lwvl::VertexArray   letterVertexArray;
        lwvl::ArrayBuffer   letterVertexBuffer;
        lwvl::ElementBuffer letterElementArray;


        lwvl::Uniform letterScale  = atlasPipeline.uniform("scale");
        lwvl::Uniform letterOffset = atlasPipeline.uniform("offset");

        // Set an initial letter.
        Glyph letter = deutsch.glyph('D');

        letterScale.set2f(letter.width, letter.height);
        letterOffset.set2f(
            (width - letter.width) * 0.5f,
            (height - letter.height) * 0.5f
        );

        // Throw the texture coordinates at the end of the buffer for easier updating.
        std::array<float, 16> atlasQuadData{
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,

            letter.texCoordL, letter.texCoordB,
            letter.texCoordR, letter.texCoordB,
            letter.texCoordR, letter.texCoordT,
            letter.texCoordL, letter.texCoordT
        };

        letterVertexArray.bind();
        letterVertexBuffer.bind();
        letterVertexBuffer.usage(lwvl::Usage::Dynamic);
        letterVertexBuffer.construct(atlasQuadData.begin(), atlasQuadData.end());
        letterVertexArray.attribute(2, GL_FLOAT, 2 * sizeof(float), 0);
        letterVertexArray.attribute(2, GL_FLOAT, 2 * sizeof(float), 8 * sizeof(float));

        letterElementArray.bind();
        letterElementArray.usage(lwvl::Usage::Static);

        std::array<uint8_t, 6> atlasQuadElements{
            0, 1, 2,
            2, 3, 0
        };
        letterElementArray.construct(atlasQuadElements.begin(), atlasQuadElements.end());

        deutsch.slot(0);
        deutsch.bind();

        atlasPipeline.uniform("atlas").set1i(0);
        //atlasPipeline.uniform("resolution").set2f(float(m_window.config.width), float(m_window.config.height));
        while (!m_window.shouldClose()) {
            Window::update();
            while (std::optional<Event> possible = m_window.pollEvent()) {
                if (!possible.has_value()) {
                    continue;
                }

                Event &concrete = possible.value();

                if (concrete.type == Event::Type::KeyRelease) {
                    KeyboardEvent &key_event = std::get<KeyboardEvent>(concrete.event);
                    if (key_event.key == GLFW_KEY_ENTER) {
                        std::cout << std::endl;
                    } else if (key_event.key == GLFW_KEY_ESCAPE) {
                        m_window.shouldClose(true);
                    }
                }

                if (concrete.type == Event::Type::TextInput) {
                    TextEvent &text_event = std::get<TextEvent>(concrete.event);
                    char      key_name    = static_cast<char>(text_event.codepoint);
                    std::cout << key_name;

                    // Change the currently rendered character.
                    letter = deutsch.glyph(key_name);

                    // Update the texture coordinates.
                    letterVertexBuffer.bind();
                    std::array<float, 8> updateData {
                        letter.texCoordL, letter.texCoordB,
                        letter.texCoordR, letter.texCoordB,
                        letter.texCoordR, letter.texCoordT,
                        letter.texCoordL, letter.texCoordT
                    };

                    letterVertexBuffer.update(updateData.begin(), updateData.end(), 8);

                    // Update the uniforms.
                    letterScale.set2f(letter.width, letter.height);
                    letterOffset.set2f(
                        (width - letter.width) * 0.5f,
                        (height - letter.height) * 0.5f
                    );
                }
            }

            lwvl::clear();
            atlasPipeline.bind();
            deutsch.bind();
            letterVertexArray.bind();
            letterVertexArray.drawElements(
                lwvl::PrimitiveMode::Triangles, 6,
                lwvl::ByteFormat::UnsignedByte
            );

            m_window.swapBuffers();
        }

        return 0;
    }
};


int main() {
    try {
        Application app(384, 384);
        return app.run();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
}
