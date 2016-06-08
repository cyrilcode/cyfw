// A helper that creates a "shadertoy"-like environment

#ifndef CYFW_SHADERTOY_H
#define CYFW_SHADERTOY_H

#include <string>
#include "cyfw/main.h"

namespace cy
{
    namespace shader
    {
        class Basic2DShader
        {
        public:
            void init(std::string code);
            void draw(ptr<Window> window);
        private:
            Shader shader;
        };
    }
}




#endif
