#ifndef CYFW_EXAMPLES_WINDOW_H
#define CYFW_EXAMPLES_WINDOW_H

#include "types.h"

#include <iostream>
#include <queue>

namespace cy
{
    double time();

    namespace window {
        int init();

        void terminate();

        enum class action {
            PRESSED = 1,
            RELEASED = 0,
            REPEAT = 2,
        };

        enum class mods {
            SHIFT = 0x0001,
            CONTROL = 0x0002,
            ALT = 0x0004,
            SUPER = 0x0008,
        };

        enum class key {
            UNKNOWN = -1,
            SPACE = 32,
            APOSTROPHE = 39,
            COMMA = 44,
            MINUS = 45,
            PERIOD = 46,
            SLASH = 47,
            KEY_0 = 48,
            KEY_1 = 49,
            KEY_2 = 50,
            KEY_3 = 51,
            KEY_4 = 52,
            KEY_5 = 53,
            KEY_6 = 54,
            KEY_7 = 55,
            KEY_8 = 56,
            KEY_9 = 57,
            SEMICOLON = 59,
            EQUAL = 61,
            A = 65,
            B = 66,
            C = 67,
            D = 68,
            E = 69,
            F = 70,
            G = 71,
            H = 72,
            I = 73,
            J = 74,
            K = 75,
            L = 76,
            M = 77,
            N = 78,
            O = 79,
            P = 80,
            Q = 81,
            R = 82,
            S = 83,
            T = 84,
            U = 85,
            V = 86,
            W = 87,
            X = 88,
            Y = 89,
            Z = 90,
            LEFT_BRACKET = 91,
            BACKSLASH = 92,
            RIGHT_BRACKET = 93,
            GRAVE_ACCENT = 96,
            WORLD_1 = 161,
            WORLD_2 = 162,
            ESCAPE = 256,
            ENTER = 257,
            TAB = 258,
            BACKSPACE = 259,
            INSERT = 260,
            DELETE = 261,
            RIGHT = 262,
            LEFT = 263,
            DOWN = 264,
            UP = 265,
            PAGE_UP = 266,
            PAGE_DOWN = 267,
            HOME = 268,
            END = 269,
            CAPS_LOCK = 280,
            SCROLL_LOCK = 281,
            NUM_LOCK = 282,
            PRINT_SCREEN = 283,
            PAUSE = 284,
            F1 = 290,
            F2 = 291,
            F3 = 292,
            F4 = 293,
            F5 = 294,
            F6 = 295,
            F7 = 296,
            F8 = 297,
            F9 = 298,
            F10 = 299,
            F11 = 300,
            F12 = 301,
            F13 = 302,
            F14 = 303,
            F15 = 304,
            F16 = 305,
            F17 = 306,
            F18 = 307,
            F19 = 308,
            F20 = 309,
            F21 = 310,
            F22 = 311,
            F23 = 312,
            F24 = 313,
            F25 = 314,
            KP_0 = 320,
            KP_1 = 321,
            KP_2 = 322,
            KP_3 = 323,
            KP_4 = 324,
            KP_5 = 325,
            KP_6 = 326,
            KP_7 = 327,
            KP_8 = 328,
            KP_9 = 329,
            KP_DECIMAL = 330,
            KP_DIVIDE = 331,
            KP_MULTIPLY = 332,
            KP_SUBTRACT = 333,
            KP_ADD = 334,
            KP_ENTER = 335,
            KP_EQUAL = 336,
            LEFT_SHIFT = 340,
            LEFT_CONTROL = 341,
            LEFT_ALT = 342,
            LEFT_SUPER = 343,
            RIGHT_SHIFT = 344,
            RIGHT_CONTROL = 345,
            RIGHT_ALT = 346,
            RIGHT_SUPER = 347,
            MENU = 348,
        };

        struct KeyEvent {
            cy::window::key key;
            int scancode;
            cy::window::action action;
            cy::window::mods mods;

            KeyEvent(window::key k, int s, window::action a, window::mods m)
                    : key(k), scancode(s), action(a), mods(m) { }
        };

        struct CharEvent {
            unsigned int codepoint;
            CharEvent(int c) : codepoint(c) {}
        };

        struct ScrollEvent {
            double xOffset;
            double yOffset;

            ScrollEvent(double x, double y) : xOffset(x), yOffset(y) { }
        };

        enum class button {
            LEFT = 0,
            RIGHT = 1,
            MIDDLE = 2,
            BUTTON_1 = 0,
            BUTTON_2 = 1,
            BUTTON_3 = 2,
            BUTTON_4 = 3,
            BUTTON_5 = 4,
            BUTTON_6 = 5,
            BUTTON_7 = 6,
            BUTTON_8 = 7,
        };

        struct MouseButtonEvent {
            cy::window::button button;
            cy::window::action action;
            cy::window::mods mods;
            vec2f location;

            MouseButtonEvent(window::button b, window::action a, window::mods m, vec2f l)
                    : button(b), action(a), mods(m), location(l) { }
        };

        struct CursorMoveEvent
        {
            vec2f location;
            CursorMoveEvent(double x, double y) : location(x,y) {}
        };

        struct CursorEnterEvent
        {
            int entered;
            CursorEnterEvent(int e) : entered(e) {}
        };

        struct ResizeEvent
        {
            vec2i size;
            ResizeEvent(int w, int h) : size(w, h) {}
        };
    }


    class Window : public std::enable_shared_from_this<Window>
    {
    public:
        Window(const int width = 640, const int height = 480,
               std::string title = "cyfw window", bool fullscreen = false);

        void toggleFullscreen();
        vec2i getFramebufferSize();
        vec2i getWindowSize();
        void setClearColor(color c);
        void clear();
        void quit();
        void closeOnEscapeKey(bool s);
        void allowToggleFullscreen(bool s);

        template <class APP>
        void run()
        {
            APP a;
            a.setWindowPointer(shared_from_this());
            a.setup();
            while (!shouldClose())
            {
                if (refresh())
                {
                    a.setup();
                }
                a.draw();
                update();
                while (!keyQueue.empty())
                {
                    a.key(keyQueue.front());
                    keyQueue.pop();
                }
                while (!scrollQueue.empty())
                {
                    a.scroll(scrollQueue.front());
                    scrollQueue.pop();
                }
                while (!mouseButtonQueue.empty())
                {
                    a.mouseButton(mouseButtonQueue.front());
                    mouseButtonQueue.pop();
                }
                while (!moveQueue.empty())
                {
                    a.cursorMove(moveQueue.front());
                    moveQueue.pop();
                }
                while (!enterQueue.empty())
                {
                    a.cursorEnter(enterQueue.front());
                    enterQueue.pop();
                }
                while (!charQueue.empty())
                {
                    a.textInput(charQueue.front());
                    charQueue.pop();
                }
                while (!resizeQueue.empty())
                {
                    a.resize(resizeQueue.front());
                    resizeQueue.pop();
                }
            }
        }

        bool closeOnEscapeKey();
        bool allowFullscreen();
        void queue(window::KeyEvent e);
        void queue(window::MouseButtonEvent e);
        void queue(window::ScrollEvent e);
        void queue(window::CursorMoveEvent e);
        void queue(window::CursorEnterEvent e);
        void queue(window::CharEvent e);
        void queue(window::ResizeEvent e);
    private:
        void initialize(bool first = false);
        int shouldClose();
        bool refresh();
        void update();
        void* windowPtr;
        std::string savedTitle;
        int savedWidth, savedHeight;
        bool isFullscreen;
        bool isWindowStateInvalid;
        color clearColor;
        bool _closeOnEscapeKey;
        bool _allowFullscreen;
        std::queue<window::KeyEvent> keyQueue;
        std::queue<window::MouseButtonEvent> mouseButtonQueue;
        std::queue<window::ScrollEvent> scrollQueue;
        std::queue<window::CursorMoveEvent> moveQueue;
        std::queue<window::CursorEnterEvent> enterQueue;
        std::queue<window::CharEvent> charQueue;
        std::queue<window::ResizeEvent> resizeQueue;

    private:
        template <class S>
        typename std::enable_if<std::is_floating_point<S>::value>::type processKeys() {
            std::cout << "Doing something";
        }

        template <class S>
        typename std::enable_if<!std::is_floating_point<S>::value>::type processKeys() {
            //doing nothing
        }
    };


}


#endif //CYFW_EXAMPLES_WINDOW_H
