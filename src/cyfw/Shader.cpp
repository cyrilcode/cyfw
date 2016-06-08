#include "Shader.h"

// Some of this code was inspired by
// https://github.com/wjakob/nanogui/blob/master/include/nanogui/glutil.h
// that file was covered by a BSD-style license.

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <sstream>
#include <iostream>
#include <Eigen/StdVector>

using namespace std;

namespace cy
{
    namespace shader
    {
        static GLuint create(GLint type, const string &name, const string &code)
        {
            if (code.empty())
                return static_cast<GLuint>(0);

            GLuint id = glCreateShader(type);
            const char *shader_string_const = code.c_str();
            glShaderSource(id, 1, &shader_string_const, nullptr);
            glCompileShader(id);
            GLint status;
            glGetShaderiv(id, GL_COMPILE_STATUS, &status);
            if (status != GL_TRUE)
            {
                char buffer[512];
                glGetShaderInfoLog(id, 512, nullptr, buffer);
                std::stringstream ss{"Error compiling "};
                if (type == GL_VERTEX_SHADER)
                    ss << "vertex shader";
                else if (type == GL_FRAGMENT_SHADER)
                    ss << "fragment shader";
                else if (type == GL_GEOMETRY_SHADER)
                    ss << "geometry shader";
                ss << std::endl << buffer << std::endl;
                throw ss.str();
            }
            return id;
        }
    }

    bool Shader::init(const std::string _name, const std::string vert, const std::string frag,
                      const std::string geom)
    {
        glGenVertexArrays(1, &vaoId);
        name = _name;
        vertexShaderId = shader::create(GL_VERTEX_SHADER, name, vert);
        fragShaderId = shader::create(GL_FRAGMENT_SHADER, name, frag);
        geomShaderId = shader::create(GL_GEOMETRY_SHADER, name, geom);

        if (!vertexShaderId || !fragShaderId) return false;
        if (!geomShaderId && !geom.empty()) return false;

        progId = glCreateProgram();

        glAttachShader(progId, vertexShaderId);
        glAttachShader(progId, fragShaderId);
        if (geomShaderId)
            glAttachShader(progId, geomShaderId);

        glLinkProgram(progId);

        GLint status;
        glGetProgramiv(progId, GL_LINK_STATUS, &status);
        if (status != GL_TRUE) {
            char buffer[512];
            glGetProgramInfoLog(progId, 512, nullptr, buffer);
            std::stringstream ss{"Error linking shader "};
            ss << name << " " << std::endl;
            ss << buffer << std::endl;
            throw ss.str();
        }
        return true;
    }

    GLint Shader::attrib(const std::string &n, bool warn) const {
        GLint id = glGetAttribLocation(progId, n.c_str());
        if (id == -1 && warn)
            std::cerr << name << ": warning: did not find attrib " << n << std::endl;
        return id;
    }

    GLint Shader::uniform(const std::string &n, bool warn) const {
        GLint id = glGetUniformLocation(progId, n.c_str());
        if (id == -1 && warn)
            std::cerr << name << ": warning: did not find uniform " << n << std::endl;
        return id;
    }

    void Shader::uploadAttrib(const string &name, uint32_t size, unsigned int dim, uint32_t compSize,
                              GLuint glType, bool integral, const int *data)
    {
        int attribId = 0;
        if (name != "indices") {
            attribId = attrib(name);
            if (attribId < 0)
                return;
        }

        GLuint bufferId;
        auto it = buffers.find(name);
        if (it != buffers.end()) {
            shader::buffer &buffer = it->second;
            bufferId = it->second.id;
            buffer.size = size;
            buffer.compSize = compSize;
        }
        else {
            glGenBuffers(1, &bufferId);
            shader::buffer buffer;
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

    void Shader::setUniform(const std::string &name, const float v, bool warn) {
        glUniform1f(uniform(name, warn), v);
    }

    void Shader::setUniform(const std::string &name, const vec2f v, bool warn) {
        glUniform2f(uniform(name, warn), v.x(), v.y());
    }
    void Shader::setUniform(const std::string &name, const mat4f &m, bool warn)
    {
        glUniformMatrix4fv(uniform(name, warn), 1, GL_FALSE, m.data());
    }
    void Shader::setUniform(const std::string &name, const aff3f &m, bool warn)
    {
        glUniformMatrix4fv(uniform(name, warn), 1, GL_FALSE, m.data());
    }

    void Shader::drawIndexed(shader::mode type, uint32_t offset_, uint32_t count_) {
        if (count_ == 0)
            return;
        size_t offset = offset_;
        size_t count = count_;

        GLenum mode = GL_TRIANGLES;
        if (type == shader::mode::TRIANGLES) {
            offset *= 3;
            count *= 3;
        }
        if (type == shader::mode::LINES) {
            mode = GL_LINES;
            offset *= 2;
            count *= 2;
        }
        if (type == shader::mode::QUADS) {
            mode = GL_QUADS;
            offset *= 4;
            count *= 4;
        }
        glDrawElements(mode, (GLsizei) count, GL_UNSIGNED_INT, (const void *) (offset * sizeof(uint32_t)));
    }


    void Shader::bind()
    {
        glUseProgram(progId);
        glBindVertexArray(vaoId);
    }

    void Shader::uploadIndices(const mat_u &m)
    {
        uploadAttrib("indices", m);
    }

    void Shader::uploadAttrib(const std::string &name, const mat_f &m)
    {
        uploadAttrib(name, static_cast<uint32_t >(m.size()), static_cast<unsigned int>(m.rows()),
                     sizeof(float), GL_FLOAT, 0, (const int *) m.data());
    }

    void Shader::uploadAttrib(const std::string &name, const mat_u &m)
    {
        uploadAttrib(name, static_cast<uint32_t >(m.size()), static_cast<unsigned int>(m.rows()),
                     sizeof(uint32_t), GL_UNSIGNED_INT, 1, (const int *) m.data());
    }

}
