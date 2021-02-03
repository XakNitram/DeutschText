#include <glad/glad.h>
#include <iostream>
#include "Core/Window.hpp"
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"


class Application {
    Window m_window;

public:
    Application(uint32_t width, uint32_t height): m_window(width, height, "TestApplication") {}

    void run() {
        while(!m_window.wantsExit()) {
            glfwPollEvents();
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

            m_window.swapBuffers();
        }
    }
};


int main() {
    Application app(800, 600);
    app.run();
    return 0;
}
