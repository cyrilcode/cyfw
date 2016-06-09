#ifndef CYFW_TYPES_H
#define CYFW_TYPES_H

#include <memory>
#include <Eigen/Dense>
#include <Eigen/Geometry>

namespace cy
{
    template<typename T>
    using ptr = std::shared_ptr<T>;

    typedef Eigen::Vector2i vec2i;

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

}

#include "Util/Color.h"

#endif
