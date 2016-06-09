
#ifndef CYFW_GUI_H
#define CYFW_GUI_H

#include "Window.h"
#include <imgui.h>

namespace cy
{
    class Gui {

    public:
        void key(window::KeyEvent);
        void mouse(window::MouseButtonEvent);
        void scroll(window::ScrollEvent);
        void character(window::CharEvent);
        ~Gui();
        void init(ptr<Window>);
        void clear();
        void draw();

        // TODO: remove this, but useful during dev
        void demo();
        bool demoOpen;

    };

}


#endif
