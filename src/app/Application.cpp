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

        Font annex("Data/Fonts/Deutsch.ttf", pixelFontSize);

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

        lwvl::VertexArray   vao;
        lwvl::ArrayBuffer   vbo;
        lwvl::ElementBuffer ebo;

        Glyph test = annex.glyph('D');

        atlasPipeline.uniform("scale").set2f(test.width, test.height);
        atlasPipeline.uniform("offset").set2f(
            (width - test.width) * 0.5f,
            (height - test.height) * 0.5f
        );

        std::array<float, 16> atlasQuadData{
            0.0f, 0.0f, test.texCoordL, test.texCoordB,
            1.0f, 0.0f, test.texCoordR, test.texCoordB,
            1.0f, 1.0f, test.texCoordR, test.texCoordT,
            0.0f, 1.0f, test.texCoordL, test.texCoordT
        };

        vao.bind();
        vbo.bind();
        vbo.usage(lwvl::Usage::Static);
        vbo.construct(atlasQuadData.begin(), atlasQuadData.end());
        vao.attribute(2, GL_FLOAT, 4 * sizeof(float), 0);
        vao.attribute(2, GL_FLOAT, 4 * sizeof(float), 2 * sizeof(float));

        ebo.bind();
        ebo.usage(lwvl::Usage::Static);

        std::array<uint8_t, 6> atlasQuadElements{
            0, 1, 2,
            2, 3, 0
        };
        ebo.construct(atlasQuadElements.begin(), atlasQuadElements.end());

        annex.slot(0);
        annex.bind();

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
                }
            }

            lwvl::clear();
            atlasPipeline.bind();
            annex.bind();
            vao.bind();
            vao.drawElements(
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
