#include "Basic2DShader.h"
#include "Resource.h"

using namespace std;

Resource Basic2DShader_vertexShader = LOAD_RESOURCE(2d_vert_glsl);
Resource Basic2DShader_fragmentShader = LOAD_RESOURCE(2d_frag_glsl);

namespace cy
{
    namespace shader
    {
        void Basic2DShader::init(std::string code) {
            shader = {};
            shader.init("shadertoy", Basic2DShader_vertexShader.toString(), Basic2DShader_fragmentShader.toString() + code + "\n");

            mat_u indices(3, 2);
            indices.col(0) << 0, 1, 2;
            indices.col(1) << 2, 1, 3;
            mat_f positions(3, 4);
            positions.col(0) << -1, -1, 0;
            positions.col(1) <<  1, -1, 0;
            positions.col(2) << -1,  1, 0;
            positions.col(3) <<  1,  1, 0;
            shader.bind();
            shader.uploadIndices(indices);
            shader.uploadAttrib("position", positions);

        }
        void Basic2DShader::draw(ptr<Window> window) {
            shader.bind();
            shader.setUniform("iResolution", window->getWindowSize());
            shader.setUniform("iGlobalTime", float(cy::time()));
            shader.drawIndexed(shader::mode::TRIANGLES, 0, 2);
        }
    }
}
