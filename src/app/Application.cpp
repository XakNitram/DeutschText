#include "pch.hpp"
#include "Core/Window.hpp"
#include "Primitives/Annex.hpp"
#include "GLDebug.hpp"


class Application {
    Window m_window;

    static void debug_gl(
            lwvl::debug::Source source, lwvl::debug::Type type,
            lwvl::debug::Severity severity, unsigned int id, int length,
            const char* message, const void* userState
    ) {
        std::cout << "[OpenGL] " << message << std::endl;
    }

public:
    Application(uint32_t width, uint32_t height): m_window(width, height, "TestApplication") {}

    int run() {
        FT_Library ft;
        if (FT_Init_FreeType(&ft)) {
            std::cerr << "Could not initialize FreeType Library." << std::endl;
            return -1;
        }

        FT_Face deutsch;
        if (FT_New_Face(ft, "Data/Fonts/Deutsch.ttf", 0, &deutsch)) {
            std::cerr << "Failed to load font from Data/Fonts/Deutsch.ttf." << std::endl;
            return -1;
        }

        // Assuming this means the height of a line is 48px.
        FT_Set_Pixel_Sizes(deutsch, 0, 48);

        lwvl::debug::GLEventListener listener(debug_gl);
        Annex annex(deutsch, 48);

        while(!m_window.wantsExit()) {
            Window::update();
            while (std::optional<Event> possible = m_window.pollEvent()) {
                if(!possible.has_value()) {
                    continue;
                }

                Event &concrete = possible.value();

                if (concrete.type == Event::Type::KeyRelease) {
                    KeyboardEvent& key_event = std::get<KeyboardEvent>(concrete.event);
                    if(key_event.key == GLFW_KEY_ENTER) {
                        std::cout << std::endl;
                    }
                }

                if (concrete.type == Event::Type::TextInput) {
                    TextEvent& text_event = std::get<TextEvent>(concrete.event);
                    char key_name = static_cast<char>(text_event.codepoint);
                    std::cout << key_name;
                }
            }

            glClear(GL_COLOR_BUFFER_BIT);
            annex.draw();

            m_window.swapBuffers();
        }

        return 0;
    }
};


int main() {
    try {
        Application app(800, 600);
        return app.run();
    } catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
}
