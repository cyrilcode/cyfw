#include "CyrilShader.h"

#include <Resource.h>
#include <cyfw/Mesh.h>


Resource CyrilShader_vertexShader = LOAD_RESOURCE(cyril_vert_glsl);
Resource CyrilShader_fragmentShader = LOAD_RESOURCE(cyril_frag_glsl);

namespace cy
{
    namespace shader
    {
        void CyrilShader::init() {
            using namespace std;

            modelMatrix = mat4f::Identity();
            shader = std::make_shared<Shader>();
            shader->init("Cyril", CyrilShader_vertexShader.toString(), CyrilShader_fragmentShader.toString());

            cubeMesh.loadFromFile("cyfw/lib/libigl/meshes/cube.obj");
            cubeMesh.setShader(shader);

            coneMesh.loadFromFile("cyfw/lib/libigl/meshes/cone.obj");
            coneMesh.setShader(shader);

            ballMesh.loadFromFile("cyfw/lib/libigl/meshes/ball.obj");
            ballMesh.setShader(shader);
        }

        void CyrilShader::bind(ptr<Window> window) {
            shader->bind();
            modelMatrix = aff3f::Identity();
            shader->setUniform("color", color{1,1,1,1});
        }

        void CyrilShader::setLightPosition(const vec3f &xyz) {
            shader->setUniform("lightPos", xyz);
        }

        void CyrilShader::setViewMatrix(const aff3f &m) {
            shader->setUniform("view", m);
        }

        void CyrilShader::setProjectionMatrix(const mat4f &m) {
            shader->setUniform("projection", m);
        }


        void CyrilShader::pushMatrix() {
            matrixStack.push(modelMatrix.matrix());
        }
        void CyrilShader::popMatrix() {
            modelMatrix = matrixStack.top();
            matrixStack.pop();
        }


        void CyrilShader::drawCube(const vec3f &whd) {
            pushMatrix();
            modelMatrix.prescale(whd);
            shader->setUniform("model", modelMatrix);
            cubeMesh.draw();
            popMatrix();
        }

        void CyrilShader::drawCube() {
            shader->setUniform("model", modelMatrix);
            cubeMesh.draw();
        }

        void CyrilShader::drawBall(const float r) {
            //shader->setUniform("model", Eigen::Scaling(r) * modelMatrix);
            pushMatrix();
            scale({r,r,r});
            shader->setUniform("model", modelMatrix);
            ballMesh.draw();
            popMatrix();
        }
//
//        void CyrilShader::drawCone(const vec2f &rh) {
//            shader->setUniform("model", modelMatrix);
//            coneMesh.draw();
//        }

        void CyrilShader::move(const vec3f &xyz) {
//            modelMatrix.translate(xyz);
            modelMatrix.translate(xyz);
        }
        void CyrilShader::scale(const vec3f &xyz) {
            modelMatrix.scale(xyz);
        }
        void CyrilShader::rotate(const quat4f &wxyz) {
            modelMatrix.rotate(wxyz);
        }

        void CyrilShader::setColor(const color &c) {
            shader->setUniform("color", c);
        }

        void CyrilShader::debug() {
            using namespace std;

            cout << modelMatrix.matrix() << endl << endl;
        }
    }
}
