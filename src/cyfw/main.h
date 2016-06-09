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
#include "Logger.h"
#include "Gui.h"

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

    // TODO: replace LOG_NAME because passing string as template argument doesn't work
    template <class APP, class LOG = NullLogger, logger::level LOG_LEVEL = logger::level::ERROR, std::string * LOG_NAME = &default_log_filename>
    int run(const int width = 640, const int height = 480,
            std::string title = "cyfw window", bool fullscreen = false)
    {
        Logger<LOG> logger(LOG_LEVEL, *LOG_NAME);
        if (!cy::window::init())
        {
            logger.error("Failed to initialize framework.");
            return EXIT_FAILURE;
        }
        atexit(cy::window::terminate);

        try
        {
            logger.verbose("Opening window");
            ptr<Window> window = std::make_shared<Window>(width, height, title, fullscreen);
            window->run<APP>();
        }
        catch (std::string error)
        {
            logger.error(error);
            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
    }
}

#endif //CYFW_MAIN_H
