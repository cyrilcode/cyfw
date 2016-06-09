#include "Gui.h"

#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>

namespace cy
{

    void Gui::key(window::KeyEvent e) {
        ImGui_ImplGlfwGL3_KeyCallback(static_cast<GLFWwindow*>(e.ptr), static_cast<int>(e.key), e.scancode,
                                      static_cast<int>(e.action),  static_cast<int>(e.mods));
    }
    void Gui::mouse(window::MouseButtonEvent e) {
        ImGui_ImplGlfwGL3_MouseButtonCallback(static_cast<GLFWwindow*>(e.ptr), static_cast<int>(e.button),
                                              static_cast<int>(e.action), static_cast<int>(e.mods));
    }
    void Gui::scroll(window::ScrollEvent e) {
        ImGui_ImplGlfwGL3_ScrollCallback(static_cast<GLFWwindow*>(e.ptr),static_cast<int>(e.xOffset),
                                         static_cast<int>(e.yOffset));
    }
    void Gui::character(window::CharEvent e) {
        ImGui_ImplGlfwGL3_CharCallback(static_cast<GLFWwindow*>(e.ptr),static_cast<int>(e.codepoint));
    }

    Gui::~Gui() {
        ImGui_ImplGlfwGL3_Shutdown();
    }

    void Gui::init(ptr<Window> w) {
        ImGui_ImplGlfwGL3_Init(static_cast<GLFWwindow*>(w->getWindowPtr()), false);
        ImGui_ImplGlfwGL3_CreateDeviceObjects();
    }

    void Gui::clear() {
        ImGui_ImplGlfwGL3_NewFrame();
    }
    void Gui::draw() {
        ImGui::Render();
    }

    void Gui::demo()
    {
        ImGui::ShowTestWindow(&demoOpen);
    }
}
