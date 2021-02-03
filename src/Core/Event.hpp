#pragma once
#include <variant>

struct KeyboardEvent { int key, scancode, mods; };
struct TextEvent { unsigned int codepoint; };
struct MouseMotionEvent { double xpos, ypos; };
struct MouseButtonEvent { int button, mods; };

using AnonymousEvent = \
std::variant<
        KeyboardEvent,
        TextEvent,
        MouseMotionEvent,
        MouseButtonEvent
>;


struct Event {
public:
    enum class Type {
        // KeyboardEvent
        KeyPress,
        KeyRelease,
        KeyRepeat,

        // TextEvent
        TextInput,

        // MouseMotionEvent
        MouseMotion,

        // MouseButtonEvent
        MouseDown,
        MouseUp,
    };

    Type type;
    AnonymousEvent event;

    Event(Type type, AnonymousEvent event);
};
