#include "pch.hpp"
#include "GLDebug.hpp"
#include "Core/Window.hpp"
#include "Primitives/Font.hpp"
#include "Primitives/Text.hpp"


class Application {
    Window m_window;

    static void debug_gl(
        lwvl::debug::Source source, lwvl::debug::Type type,
        lwvl::debug::Severity severity, unsigned int id, int length,
        const char *message, const void *userState
    ) {
#ifdef NDEBUG
        if (type == lwvl::debug::Type::ERROR)
#endif
        {
            std::cout << "[OpenGL] " << message << std::endl;
        }
    }

public:
    Application(uint32_t width, uint32_t height) : m_window(width, height, "DeutschText") {}

    int run() {
        //const uint32_t pixelFontSize = 64;
        const uint32_t pixelFontSize = 384;
        //const uint32_t pixelFontSize = 768;

        lwvl::debug::GLEventListener listener(debug_gl);

        Font deutsch("Data/Fonts/Deutsch.ttf", pixelFontSize);
        //Font deutsch("Data/Fonts/SourceCodePro.ttf", pixelFontSize);
        deutsch.slot(0);

        TextFactory factory(deutsch);

        const auto width = static_cast<float>(m_window.config.width);
        const auto height = static_cast<float>(m_window.config.height);
        //factory.program.uniform("resolution").set2f(width, height);
        factory.program.uniform("projection").set2DOrthographic(
            height, 0.0f,
            width, 0.0f
        );

        Text tagline = factory.create("Deutsch", 0.0f, 0.0f, 1.0f);
        {
            float tagWidth = tagline.Width();
            const float scale = (0.75f * width) / tagWidth;
            const float tagHeight = tagline.Height() * scale;
            tagWidth *= scale;

            tagline.Scale(scale);
            tagline.update(
                (width - tagWidth) * 0.5f,
                (height - tagHeight) * 0.5f
            );
        }

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
                    char key_name = static_cast<char>(text_event.codepoint);
                    std::cout << key_name;
                }
            }

            lwvl::clear();
            tagline.draw();
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
