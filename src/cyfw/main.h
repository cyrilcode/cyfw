#ifndef CYFW_MAIN_H
#define CYFW_MAIN_H

#ifndef GLSL_VERSION
#define GLSL_VERSION "#version 330\n"
#endif
// Use this to pass raw GLSL strings
#define glsl(x) GLSL_VERSION #x

#include <iostream>

#include "types.h"
#include "Window.h"
#include "Shader.h"
#include "Camera.h"
#include "VideoGrabber.h"
#include "Texture.h"

namespace cy
{

    class App
    {
    public:
        void draw() {}
        void setup() {}
        void setWindowPointer(ptr<Window> w)  { window = w; }
        void key(window::KeyEvent) {}
        void scroll(window::ScrollEvent) {}
        void mouseButton(window::MouseButtonEvent) {}
        void cursorMove(window::CursorMoveEvent) {}
        void cursorEnter(window::CursorEnterEvent) {}
        void textInput(window::CharEvent) {}
        void resize(window::ResizeEvent) {}
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
