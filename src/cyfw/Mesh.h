#ifndef CYFW_MESH_H
#define CYFW_MESH_H

#include "types.h"
#include "Shader.h"
#include <unordered_map>
#include <string>

namespace cy
{
    namespace mesh
    {

        struct buffer
        {
            unsigned int id;
            unsigned int type;
            unsigned int dim;
            unsigned int compSize;
            unsigned int size;
        };
    }

    class Mesh {
        mat_f vertices;
        mat_f normals;
        mat_u indices;
        bool dirty;
        bool faceBased;
        unsigned int vaoId;
        std::unordered_map<std::string, mesh::buffer> buffers;
        void uploadAttrib(const std::string &name, uint32_t size, unsigned int dim, uint32_t compSize,
                          unsigned int glType, bool integral, const int *data);
        ptr<Shader> shader;
    public:
        Mesh() : Mesh({}, {}, {}) {}
        Mesh(mat_f V, mat_u I, mat_f N);
        void draw();
        void upload();
        void uploadAttrib(const std::string &name, const mat_f &m);
        void uploadAttrib(const std::string &name, const mat_u &m);
        void setShader(ptr<Shader>);
        void setPerFaceNormals();
        void loadFromFile(std::string filename);
    };

}

#endif
