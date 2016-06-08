#include "Basic3DShader.h"

#include <Resource.h>

Resource Basic3DShader_vertexShader = LOAD_RESOURCE(3d_vert_glsl);
Resource Basic3DShader_fragmentShader = LOAD_RESOURCE(3d_frag_glsl);

namespace cy
{
    namespace shader
    {
        void Basic3DShader::init()
        {
            shader = {};
            shader.init("3d", Basic3DShader_vertexShader.toString(), Basic3DShader_fragmentShader.toString());

            mat_f positions(3, 8);
            // front
            positions.col(0) << -0.5, -0.5, 0.5;
            positions.col(1) <<  0.5, -0.5, 0.5;
            positions.col(2) <<  0.5,  0.5, 0.5;
            positions.col(3) << -0.5,  0.5, 0.5;
            // back
            positions.col(4) << -0.5, -0.5, -0.5;
            positions.col(5) <<  0.5, -0.5, -0.5;
            positions.col(6) <<  0.5,  0.5, -0.5;
            positions.col(7) << -0.5,  0.5, -0.5;

            mat_f colors(3, 8);
            colors.col(0) << 1, 0, 0;
            colors.col(1) << 0, 1, 0;
            colors.col(2) << 0, 0, 1;
            colors.col(3) << 1, 0, 0;
            colors.col(4) << 1, 1, 0;
            colors.col(5) << 0, 1, 1;
            colors.col(6) << 1, 0, 1;
            colors.col(7) << 1, 1, 1;

            mat_u indices(3, 12);
            // front
            indices.col(0) << 0, 1, 2;
            indices.col(1) << 2, 3, 0;
            // top
            indices.col(2) << 1, 5, 6;
            indices.col(3) << 6, 2, 1;
            // back
            indices.col(4) << 7, 6, 5;
            indices.col(5) << 5, 4, 7;
            // bottom
            indices.col(6) << 4, 0, 3;
            indices.col(7) << 3, 7, 4;
            // left
            indices.col(8) << 4, 5, 1;
            indices.col(9) << 1, 0, 4;
            // right
            indices.col(10) << 3, 2, 6;
            indices.col(11) << 6, 7, 3;

            shader.bind();
            shader.uploadIndices(indices);
            shader.uploadAttrib("position", positions);
            shader.uploadAttrib("normal", positions);
            shader.uploadAttrib("color", colors);
        }

        void Basic3DShader::bind(ptr <Window> window)
        {
            window->setClearColor({0.0f,0.0f,0.0f,1.0f});
            window->clear();
            shader.bind();
        }

        void Basic3DShader::drawCube() {
            shader.drawIndexed(shader::mode::TRIANGLES, 0, 12);
        }

        void Basic3DShader::setProjectionMatrix(const mat4f &m)
        {
            shader.setUniform("projection", m);
        }

        void Basic3DShader::setViewMatrix(const aff3f &m)
        {
            shader.setUniform("view", m);
        }

        void Basic3DShader::setLightPosition(const vec3f &p) {
            shader.setUniform("lightPos", p);
        }

    }
}