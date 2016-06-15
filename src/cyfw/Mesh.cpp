#include "Mesh.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <sstream>
#include <iostream>
#include <Eigen/StdVector>
#include <igl/per_vertex_normals.h>
#include <igl/per_face_normals.h>
#include <igl/per_corner_normals.h>


using namespace std;

namespace cy
{
    Mesh::Mesh(mat_f V, mat_u I)
    : vertices(V), indices(I), dirty(true), faceBased(false)
    {
        glGenVertexArrays(1, &vaoId);
    }

    void Mesh::setPerFaceNormals() {
        normals = {};
        mat_f tempV = vertices.transpose();
        mat_u tempU = indices.transpose();
        mat_f tempN;
        igl::per_face_normals(tempV, tempU, tempN);
        normals = tempN.transpose();
        if (normals.cols() == vertices.cols())
        {
            faceBased = false;
        }
        else
        {
            faceBased = true;
            mat_f tempV(3, normals.cols() * 3);
            mat_u tempU(3, normals.cols());
            mat_f tempN(3, normals.cols() * 3);
            int counter = 0;
            for (int i = 0; i < indices.cols(); ++i)
            {
                for (int j = 0; j < 3; ++j)
                {
                    for (int k = 0; k < 3; ++k)
                    {
                        tempV(k, counter) = vertices(k, indices(j, i));
                        tempN(k, counter) = normals(k, i);
                    }
                    tempU(j, i) = counter++;
                }
            }
            indices = tempU;
            vertices = tempV;
            normals = tempN;
        }
        dirty = true;
    }

    void Mesh::upload() {
        shader->bind();
        glBindVertexArray(vaoId);
        uploadAttrib("indices", indices);
        uploadAttrib("position", vertices);
        uploadAttrib("normal", normals);
        dirty = false;
    }

    void Mesh::draw() {
        if (dirty)
            upload();

        glBindVertexArray(vaoId);
        glDrawElements(GL_TRIANGLES, (GLsizei) indices.size(), GL_UNSIGNED_INT, 0);
    }

    void Mesh::setShader(ptr<Shader> s) {
        shader = s;
        dirty = true;
    }

    void Mesh::uploadAttrib(const std::string &name, const mat_f &m)
    {
        uploadAttrib(name, static_cast<uint32_t >(m.size()), static_cast<unsigned int>(m.rows()),
                     sizeof(float), GL_FLOAT, 0, (const int *) m.data());
    }

    void Mesh::uploadAttrib(const std::string &name, const mat_u &m)
    {
        uploadAttrib(name, static_cast<uint32_t >(m.size()), static_cast<unsigned int>(m.rows()),
                     sizeof(uint32_t), GL_UNSIGNED_INT, 1, (const int *) m.data());
    }

    void Mesh::uploadAttrib(const string &name, uint32_t size, unsigned int dim, uint32_t compSize,
                              GLuint glType, bool integral, const int *data)
    {
        int attribId = 0;
        if (name != "indices") {
            attribId = shader->attrib(name);
            if (attribId < 0)
                return;
        }

        GLuint bufferId;
        auto it = buffers.find(name);
        if (it != buffers.end()) {
            mesh::buffer &buffer = it->second;
            bufferId = it->second.id;
            buffer.size = size;
            buffer.compSize = compSize;
        }
        else {
            glGenBuffers(1, &bufferId);
            mesh::buffer buffer;
            buffer.id = bufferId;
            buffer.type = glType;
            buffer.dim = dim;
            buffer.compSize = compSize;
            buffer.size = size;
            buffers[name] = buffer;
        }
        size_t totalSize = (size_t) size * (size_t) compSize;

        if (name == "indices") {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, totalSize, data, GL_STATIC_DRAW);
        } else {
            glBindBuffer(GL_ARRAY_BUFFER, bufferId);
            glBufferData(GL_ARRAY_BUFFER, totalSize, data, GL_STATIC_DRAW);
            if (size == 0) {
                glDisableVertexAttribArray(attribId);
            } else {
                glEnableVertexAttribArray(attribId);
                glVertexAttribPointer(attribId, dim, glType, integral, 0, 0);
            }
        }
    }
}
