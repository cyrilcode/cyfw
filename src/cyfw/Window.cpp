
#include "Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <string>

using namespace std;

namespace cy
{
    double time()
    {
        return glfwGetTime();
    }

    namespace window
    {
        GLFWmonitor* primaryMonitor;

        static void error_callback(int error, const char* description)
        {
            string msg{to_string(error) + ": " + string(description)};
            throw msg;
        }

        int init()
        {
            glfwSetErrorCallback(error_callback);
            int status = glfwInit();
            if (status)
            {
                primaryMonitor = glfwGetPrimaryMonitor();
            }
            return status;
        }
        void terminate()
        {
            glfwTerminate();
        }

        static void key_callback(GLFWwindow* w, int key, int s, int action, int mods)
        {
            if (action == GLFW_PRESS)
            {
                if (key == GLFW_KEY_F && mods == GLFW_MOD_SUPER &&
                        static_cast<Window*>(glfwGetWindowUserPointer(w))->allowFullscreen())
                {
                    static_cast<Window*>(glfwGetWindowUserPointer(w))->toggleFullscreen();
                    return;
                }
                if (key == GLFW_KEY_ESCAPE &&
                        static_cast<Window*>(glfwGetWindowUserPointer(w))->closeOnEscapeKey())
                {
                    static_cast<Window*>(glfwGetWindowUserPointer(w))->quit();
                    return;
                }
            }
            static_cast<Window*>(glfwGetWindowUserPointer(w))->queue(
                    window::KeyEvent(
                            w,
                            static_cast<window::key>(key), s,
                            static_cast<window::action>(action), static_cast<window::mods>(mods)));
        }

        static void mouse_button_callback(GLFWwindow* w, int button, int action, int mods)
        {
            double xpos, ypos;
            glfwGetCursorPos(w, &xpos, &ypos);
            static_cast<Window*>(glfwGetWindowUserPointer(w))->queue(
                    window::MouseButtonEvent(w,
                            static_cast<window::button>(button),
                            static_cast<window::action>(action),
                            static_cast<window::mods>(mods),
                    vec2f({xpos, ypos})));
        }

        static void scroll_callback(GLFWwindow* w, double xOffset, double yOffset)
        {
            static_cast<Window*>(glfwGetWindowUserPointer(w))->queue(window::ScrollEvent(w, xOffset, yOffset));
        }

        static void cursor_move_callback(GLFWwindow* w, double xPos, double yPos)
        {
            static_cast<Window*>(glfwGetWindowUserPointer(w))->queue(window::CursorMoveEvent(xPos, yPos));
        }
        static void cursor_enter_callback(GLFWwindow* w, int entered)
        {
            static_cast<Window*>(glfwGetWindowUserPointer(w))->queue(window::CursorEnterEvent(entered));
        }

        static void character_callback(GLFWwindow* w, unsigned int codepoint)
        {
            static_cast<Window*>(glfwGetWindowUserPointer(w))->queue(window::CharEvent(w, codepoint));
        }

        static void resize_callback(GLFWwindow* w, int width, int height)
        {
            static_cast<Window*>(glfwGetWindowUserPointer(w))->queue(window::ResizeEvent(width, height));
        }

        namespace glfw
        {

            GLFWwindow* open_window(GLFWmonitor* monitor, const int width,
                                    const int height, const char* title, bool fullscreen)
            {
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
                glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
                if (fullscreen)
                {
                    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
                    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
                    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
                    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
                    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
                    return glfwCreateWindow(mode->width, mode->height, title, monitor, NULL);
                }
                return glfwCreateWindow(width, height, title, NULL, NULL);
            }

        }

    }

    Window::Window(const int width, const int height, std::string title, bool fullscreen)
        : clearColor{0.949, 0.949, 0.949, 1}, isFullscreen{fullscreen}, isWindowStateInvalid{false},
          savedWidth{width}, savedHeight{height}, savedTitle{title}, _closeOnEscapeKey{true},
          _allowFullscreen{true}
    {
        windowPtr = window::glfw::open_window(window::primaryMonitor, width, height, title.c_str(), fullscreen);
        if (!windowPtr) throw string("Failed to create GLFW window");

        // pass true to initialize GLAD on first run only
        initialize(true);
    }

    vec2i Window::getFramebufferSize() {
        int width, height;
        glfwGetFramebufferSize(static_cast<GLFWwindow*>(windowPtr), &width, &height);
        return {width, height};
    }
    vec2i Window::getWindowSize() {
        int width, height;
        glfwGetWindowSize(static_cast<GLFWwindow*>(windowPtr), &width, &height);
        return {width, height};
    }
    void Window::quit() {
        glfwSetWindowShouldClose(static_cast<GLFWwindow*>(windowPtr), 1);
    }

    int Window::shouldClose() {
        return glfwWindowShouldClose(static_cast<GLFWwindow*>(windowPtr));
    }

    void Window::update() {
        glfwSwapBuffers(static_cast<GLFWwindow *>(windowPtr));
    }

    void Window::initialize(bool first)
    {
        glfwMakeContextCurrent(static_cast<GLFWwindow *>(windowPtr));
        glfwSwapInterval(1);
        glfwSetWindowUserPointer(static_cast<GLFWwindow *>(windowPtr), this);
        glfwSetKeyCallback(static_cast<GLFWwindow *>(windowPtr), window::key_callback);
        glfwSetScrollCallback(static_cast<GLFWwindow *>(windowPtr), window::scroll_callback);
        glfwSetMouseButtonCallback(static_cast<GLFWwindow *>(windowPtr), window::mouse_button_callback);
        glfwSetCursorPosCallback(static_cast<GLFWwindow *>(windowPtr), window::cursor_move_callback);
        glfwSetCursorEnterCallback(static_cast<GLFWwindow *>(windowPtr), window::cursor_enter_callback);
        glfwSetCharCallback(static_cast<GLFWwindow *>(windowPtr), window::character_callback);
        glfwSetFramebufferSizeCallback(static_cast<GLFWwindow *>(windowPtr), window::resize_callback);
        if (first) gladLoadGL();
        vec2i fbSize = getFramebufferSize();
        glViewport(0, 0, static_cast<GLuint>(fbSize.x()), static_cast<GLuint>(fbSize.y()));
        glEnable(GL_DEPTH_TEST);
        clear();
        update();
        clear();
    }

    void Window::toggleFullscreen() {
        isFullscreen = !isFullscreen;
        isWindowStateInvalid = true;
    }

    void Window::setClearColor(color c) {
        clearColor = c;
    }

    void Window::clear() {
        glClearColor(clearColor.r(), clearColor.g(), clearColor.b(), clearColor.a());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    bool Window::refresh() {
        if (isWindowStateInvalid)
        {
            if (isFullscreen)
            {
                // switching to fullscreen, so save current window size
                glfwGetWindowSize(static_cast<GLFWwindow *>(windowPtr), &savedWidth, &savedHeight);
            }
            glfwDestroyWindow(static_cast<GLFWwindow *>(windowPtr));
            windowPtr = window::glfw::open_window(window::primaryMonitor, savedWidth, savedHeight, savedTitle.c_str(), isFullscreen);
            if (!windowPtr) throw string("Window creation failed");

            initialize();
            isWindowStateInvalid = false;
            return true;
        }
        glfwPollEvents();
        return false;
    }

    bool Window::allowFullscreen() {
        return _allowFullscreen;
    }
    bool Window::closeOnEscapeKey() {
        return _closeOnEscapeKey;
    }
    void Window::closeOnEscapeKey(bool s) {
        _closeOnEscapeKey = s;
    }
    void Window::allowToggleFullscreen(bool s) {
        _allowFullscreen = s;
    }

    void Window::queue(window::KeyEvent e) {
        keyQueue.push(e);
    }
    void Window::queue(window::MouseButtonEvent e) {
        mouseButtonQueue.push(e);
    }
    void Window::queue(window::ScrollEvent e) {
        scrollQueue.push(e);
    }
    void Window::queue(window::CursorMoveEvent e) {
        moveQueue.push(e);
    }
    void Window::queue(window::CursorEnterEvent e) {
        enterQueue.push(e);
    }
    void Window::queue(window::CharEvent e) {
        charQueue.push(e);
    }
    void Window::queue(window::ResizeEvent e) {
        resizeQueue.push(e);
    }
}