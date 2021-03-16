#pragma once
#include "pch.hpp"
#include "Event.hpp"


extern constexpr size_t event_queue_capacity = 3;


class Window {
public:
    struct Config {
        uint32_t width;
        uint32_t height;
    };

private:
    Config m_config;
    GLFWwindow* m_window;

    std::vector<Event> m_events;

    static Window* getState(GLFWwindow* window);

    void terminate();

public:
    Window(uint32_t width, uint32_t height, const char* title, GLFWmonitor* monitor = nullptr);
    bool wantsExit();
    void swapBuffers();
    static void update();
    static void clear();

    std::optional<Event> pollEvent();
};
