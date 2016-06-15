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
            modelMatrix = mat4f::Identity();
            shader = std::make_shared<Shader>();
            shader->init("Cyril", CyrilShader_vertexShader.toString(), CyrilShader_fragmentShader.toString());


            // Cube Primitive
            // Vertex [0 - 7]
            // Indices [0 - 12]

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

//            mat_f positions(8, 3);
//            mat_u indices(12, 3);
//
//            positions.row(0) << -0.5, -0.5, 0.5;
//            positions.row(1) <<  0.5, -0.5, 0.5;
//            positions.row(2) <<  0.5,  0.5, 0.5;
//            positions.row(3) << -0.5,  0.5, 0.5;
//            positions.row(4) << -0.5, -0.5, -0.5;
//            positions.row(5) <<  0.5, -0.5, -0.5;
//            positions.row(6) <<  0.5,  0.5, -0.5;
//            positions.row(7) << -0.5,  0.5, -0.5;
//            indices.row(0) << 0, 1, 2;
//            indices.row(1) << 2, 3, 0;
//            indices.row(2) << 1, 5, 6;
//            indices.row(3) << 6, 2, 1;
//            indices.row(4) << 7, 6, 5;
//            indices.row(5) << 5, 4, 7;
//            indices.row(6) << 4, 0, 3;
//            indices.row(7) << 3, 7, 4;
//            indices.row(8) << 4, 5, 1;
//            indices.row(9) << 1, 0, 4;
//            indices.row(10) << 3, 2, 6;
//            indices.row(11) << 6, 7, 3;

            cubeMesh = {positions, indices};
            cubeMesh.setShader(shader);
            cubeMesh.setPerFaceNormals();

            shader->bind();
//            shader->uploadIndices(indices);
//            shader->uploadAttrib("position", positions);
//            shader->uploadAttrib("normal", normals);
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

        void CyrilShader::drawCube(const vec3f &whd) {
            // apply whd scale to model matrix
            Eigen::Affine3f temp = modelMatrix * Eigen::Scaling(whd);
            shader->setUniform("model", temp);
            // draw cube elements
            //shader.drawIndexed(shader::mode::TRIANGLES, 0, 12);

            cubeMesh.draw();
        }

        void CyrilShader::move(const vec3f &xyz) {
            modelMatrix *= Eigen::Translation<float,3>(xyz);
        }

        void CyrilShader::setColor(const color &c) {
            shader->setUniform("color", c);
        }
    }
}
