#ifndef CYFW_TYPES_H
#define CYFW_TYPES_H

#include <memory>
#include <Eigen/Dense>
#include <Eigen/Geometry>

namespace cy
{
    template<typename T>
    using ptr = std::shared_ptr<T>;

    typedef Eigen::Vector2f vec2f;
    typedef Eigen::Vector3f vec3f;
    typedef Eigen::Vector4f vec4f;
    typedef Eigen::VectorXi vec_u;

    typedef Eigen::Matrix3f mat3f;
    typedef Eigen::Matrix4f mat4f;
    typedef Eigen::MatrixXi mat_u;
    typedef Eigen::MatrixXf mat_f;

    typedef Eigen::Quaternionf quat4f;
    typedef Eigen::Affine3f aff3f;

    class color
    {
        vec4f _v;
    public:
        inline color(vec4f value) : _v(value) {}
        inline float& r() { return _v.x(); }
        inline float& g() { return _v.y(); }
        inline float& b() { return _v.z(); }
        inline float& a() { return _v.w(); }
        inline float r() const { return _v.x(); }
        inline float g() const { return _v.y(); }
        inline float b() const { return _v.z(); }
        inline float a() const { return _v.w(); }
        inline vec4f asVec4f() const { return _v; }
        inline vec3f asVec3f() const { return {_v.x(), _v.y(), _v.z()}; };
    };


}


#endif
