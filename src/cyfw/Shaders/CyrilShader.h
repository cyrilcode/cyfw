
#ifndef CYFW_CYRILSHADER_H
#define CYFW_CYRILSHADER_H

#include "cyfw/main.h"

namespace cy
{
    namespace shader
    {

        class CyrilShader {
        public:
            void init();
            void bind(ptr<Window> window);

            void move(const vec3f);
            void rotate(const quat4f);
            void scale(const vec3f);
            void pushMatrix();
            void popMatrix();

            void drawCube(const vec3f whd);
            void drawBall(const float r);
            void setBallDetail(const float d);
            void drawSphere(const float r);
            void setSphereDetail(const float d);
            void drawPeg(const vec2f rh);
            void drawCone(const vec2f rh);
            void drawRect(const vec2f wh);
            void drawLine(const vec2f xy);
            void drawGrid(const float);
            // TODO: custom shapes?
            // Cyril did allow custom shapes using
            // old OpenGL stuff from openFrameworks
            // i.e. startShape() ... vert() ... endShape()

            void setFill();
            void setNoFill();
            void setStrokeSize(const float);
            void setColor(color);
            void setBackground(color);
            void setBackground(Palette);

            void setLightColor(color diffuse, color ambient);
            void setLightPosition(vec3f xyz);
            void enableLighting();
            void disableLighting();

        private:
            Shader shader;
        };
    }
}


#endif //CYFW_CYRILSHADER_H
