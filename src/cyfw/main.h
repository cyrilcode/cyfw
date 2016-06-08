#ifndef CYFW_MAIN_H
#define CYFW_MAIN_H


#include <iostream>

#include "types.h"
#include "Window.h"
#include "Shader.h"
#include "Camera.h"

namespace cy
{

    class App
    {
    public:
        void setWindowPointer(ptr<Window> w)  { window = w; }
        void key(window::KeyEvent) {}
        void scroll(window::ScrollEvent) {}
        void mouseButton(window::MouseButtonEvent) {}
        void cursorMove(window::CursorMoveEvent) {}
        void cursorEnter(window::CursorEnterEvent) {}
    protected:
        ptr<Window> window;
    };


    template <class APP>
    int run(const int width = 640, const int height = 480,
            std::string title = "cyfw window", bool fullscreen = false)
    {
        if (!cy::window::init())
            return EXIT_FAILURE;
        atexit(cy::window::terminate);

        try
        {
            ptr<Window> window = std::make_shared<Window>(width, height, title, fullscreen);
            window->run<APP>();
        }
        catch (std::string error)
        {
            std::cerr << error << std::endl;
            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
    }
}

#endif //CYFW_MAIN_H
