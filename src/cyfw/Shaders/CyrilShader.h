
#ifndef CYFW_CYRILSHADER_H
#define CYFW_CYRILSHADER_H

#include <stack>
#include <cyfw/Mesh.h>
#include "cyfw/main.h"
#include "cyfw/Util/Palette.h"

namespace cy
{
    namespace shader
    {

        class CyrilShader {
        public:
            CyrilShader() : currentColor{1,1,1,1}
            {}
            void init();
            void bind(ptr<Window> window);

            void move(const vec3f &xyz);
            void rotate(const quat4f &wxyz);
            void scale(const vec3f &xyz);
            void pushMatrix();
            void popMatrix();

            void drawCube();
            void drawCube(const vec3f &whd);
            void drawBall(const float r);
            void setBallDetail(const float d);
            void drawSphere(const float r);
            void setSphereDetail(const float d);
            void drawPeg(const vec2f &rh);
            void drawCone(const vec2f &rh);
            void drawRect(const vec2f &wh);
            void drawLine(const vec2f &xy);
            void drawGrid(const float);
            // TODO: custom shapes?
            // Cyril did allow custom shapes using
            // old OpenGL stuff from openFrameworks
            // i.e. startShape() ... vert() ... endShape()

            void setFill();
            void setNoFill();
            void setStrokeSize(const float);
            void setColor(const color &);
            void setBackground(const color &);
            void setBackground(const Palette<ListPalette> &);

            void setLightColor(const color &diffuse, const color &ambient);
            void setLightPosition(const vec3f &xyz);
            void enableLighting();
            void disableLighting();

            void setProjectionMatrix(const mat4f &m);
            void setViewMatrix(const aff3f &m);

            void debug();
        private:
            ptr<Shader> shader;
            std::stack<mat4f> matrixStack;
            aff3f modelMatrix;
            color currentColor;
            Mesh cubeMesh;
            Mesh coneMesh;
            Mesh ballMesh;
        };
    }
}


#endif //CYFW_CYRILSHADER_H
