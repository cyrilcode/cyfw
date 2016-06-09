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

namespace cy
{

    namespace math
    {
        const static float PI     = 3.14159265359;
        const static float TWO_PI = 6.28318530718;

        inline float map(const float v, const float iMin, const float iMax, const float oMin, const float oMax)
        {
            // TODO: check for divide by zero
            return (v - iMin) / (iMax - iMin) * (oMax - oMin) + oMin;
        }

        // TODO: noise and random
        float noise(const float x);
        float noise(const float x, const float y);
        float noise(const float x, const float y, const float z);
        float rand();
        float rand(float maximum);
        float rand(float minimum, float maximum);

        inline clamp(float v, float min, float max) {
            return v < min ? min : value > max ? max : v;
        }
        inline float normalize(float v, float minimum, float maximum) {
            clamp((v - minimum) / (maximum - minimum), 0, 1);
        }
    }

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
