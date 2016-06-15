#include "Mesh.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <sstream>
#include <iostream>
#include <Eigen/StdVector>
#include <igl/per_vertex_normals.h>
#include <igl/per_face_normals.h>
#include <igl/per_corner_normals.h>

#include <igl/slice.h>
#include <igl/unzip_corners.h>
#include <igl/readOBJ.h>

using namespace std;

namespace cy
{
    Mesh::Mesh(mat_f V, mat_u I, mat_f N)
    : vertices(V), indices(I), normals(N), dirty(true), faceBased(false)
    {
        glGenVertexArrays(1, &vaoId);
    }

    void Mesh::loadFromFile(std::string filename) {
        Eigen::MatrixXf V, TC, N;
        Eigen::MatrixXi F,FTC,FN;
        igl::readOBJ(filename,V,TC,N,F,FTC,FN);
        if(FTC.size() == 0)
        {
            FTC = F;
        }
        Eigen::MatrixXi U,G,J;
        igl::unzip_corners<Eigen::MatrixXi>({F,FTC,FN},U,G,J);
        // New mesh vertices and texture coordinates indexed by G
        mat_f GV = igl::slice(Eigen::MatrixXf(V),U.col(0),1);
        mat_f GTC = igl::slice(Eigen::MatrixXf(TC),U.col(1),1);
        mat_f GN = igl::slice(Eigen::MatrixXf(N),U.col(2),1);

//        cout << "Unzipped corners from OBJ:" << endl;
//        cout << GV.transpose() << endl;
//        cout << GN.transpose() << endl;
//        cout << GTC.transpose() << endl;
//        cout << G.transpose() << endl;

        indices = G.transpose();
        normals = GN.transpose();
        vertices = GV.transpose();
    }

    // TODO: fix this using the unzip method of deduping vertices
    void Mesh::setPerFaceNormals() {
        normals = {};
        mat_f tempV = vertices.transpose();
        mat_u tempU = indices.transpose();
        mat_f tempN;
        igl::per_face_normals(tempV, tempU, tempN);
        normals = tempN.transpose();
        cout << "Before: " << endl;
        cout << indices << endl;
        cout << vertices << endl;
        cout << normals << endl;
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
            cout << "Processed: " << endl;
            cout << indices << endl;
            cout << vertices << endl;
            cout << normals << endl;
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
