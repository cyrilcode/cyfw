
#ifndef CYFW_BASIC3DSHADER_H
#define CYFW_BASIC3DSHADER_H

#include "cyfw/main.h"

namespace cy
{
    namespace shader
    {
        class Basic3DShader {
        public:
            void init();
            void bind(ptr<Window> window);
            void drawCube();
            void setProjectionMatrix(const mat4f &m);
            void setViewMatrix(const aff3f &m);
        private:
            Shader shader;
        };
    }
}


#endif
