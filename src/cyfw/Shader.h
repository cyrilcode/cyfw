
#ifndef CYFW_SHADER_H
#define CYFW_SHADER_H

#include "main.h"
#include <unordered_map>

namespace cy
{
    namespace shader
    {
        enum class mode {
            TRIANGLES, LINES, QUADS
        };

        struct buffer
        {
            unsigned int id;
            unsigned int type;
            unsigned int dim;
            unsigned int compSize;
            unsigned int size;
        };
    }

    class Shader
    {
    public:
        bool init(const std::string name, const std::string vert, const std::string frag, const std::string geom = "");
        void bind();

        void uploadAttrib(const std::string &name, const mat_f &m);
        void uploadAttrib(const std::string &name, const mat_u &m);
        void uploadIndices(const mat_u &m);

        int attrib(const std::string &name, bool warn = true) const;
        int uniform(const std::string &name, bool warn = true) const;

        void setUniform(const std::string &name, const float v, bool warn = true);
        void setUniform(const std::string &name, const vec2i v, bool warn = true);
        void setUniform(const std::string &name, const vec2f v, bool warn = true);
        void setUniform(const std::string &name, const vec3f v, bool warn = true);
        void setUniform(const std::string &name, const mat4f &m, bool warn = true);
        void setUniform(const std::string &name, const aff3f &m, bool warn = true);

        /// Draw a sequence of primitives using a previously uploaded index buffer
        void drawIndexed(shader::mode type, uint32_t offset, uint32_t count);

    private:
        void uploadAttrib(const std::string &name, uint32_t size, unsigned int dim, uint32_t compSize,
                          unsigned int glType, bool integral, const int *data);

        std::string name;
        unsigned int vertexShaderId;
        unsigned int fragShaderId;
        unsigned int geomShaderId;
        unsigned int progId;
        unsigned int vaoId;
        std::unordered_map<std::string, shader::buffer> buffers;
    };

}


#endif
