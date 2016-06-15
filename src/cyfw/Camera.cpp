// Some of this code was originally sourced from Eigen camera
// which is covered by Mozilla Public License v. 2.0.
// https://fossies.org/dox/eigen-3.2.8/camera_8h_source.html


#include "Camera.h"

#include <iostream>
using namespace std;

namespace cy
{

    Camera::Camera() : vpX(0), vpY(0), hasViewChanged(true), hasProjChanged(true),
                       scrollSensitivity{0.1f, 0.1f}, dragSensitivity{0.01f, 0.01f}, dragging{false}
    {
        viewM.setIdentity();
//        fovY = M_PI/3.0f;
        fovY = 0.785f;
        nearDist = 1.0f;
        farDist = 100.0f;
        setOrientation({0, 0, 1, 0});
        setPosition({0.0f, 0.0f, -3.0f});
        setTarget(vec3f::Zero());
    }

    void Camera::setViewport(unsigned int x, unsigned int y, unsigned int w, unsigned int h)
    {
        vpX = x;
        vpY = y;
        vpWidth = w;
        vpHeight = h;
        hasProjChanged = true;
    }

    void Camera::setViewport(unsigned int w, unsigned int h)
    {
        vpWidth = w;
        vpHeight = h;
        hasProjChanged = true;
    }

    void Camera::setViewport(vec2f dim)
    {
        vpWidth = dim.x();
        vpHeight = dim.y();
        hasProjChanged = true;
    }
    void Camera::setViewport(vec2i dim)
    {
        vpWidth = dim.x();
        vpHeight = dim.y();
        hasProjChanged = true;
    }

    vec3f Camera::getPosition()
    {
        return position;
    }

    void Camera::setPosition(const vec3f &p)
    {
        position = p;
        hasViewChanged = true;
    }

    vec3f Camera::up() const
    {
        return orientation * vec3f::UnitY();
    }
    vec3f Camera::direction() const {
        return - (orientation * vec3f::UnitZ());
    }
    vec3f Camera::right() const {
        return orientation * vec3f::UnitX();
    }

    quat4f Camera::getOrientation()
    {
        return orientation;
    }

    void Camera::setOrientation(const quat4f &q)
    {
        orientation = q;
        hasViewChanged = true;
    }

    const aff3f& Camera::viewMatrix()
    {
        if(hasViewChanged)
        {
            quat4f q = orientation.conjugate();
            viewM.linear() = q.toRotationMatrix();
            viewM.translation() = - (viewM.linear() * position);

            hasViewChanged = false;
        }
        return viewM;
    }

    const mat4f& Camera::projectionMatrix()
    {
        if (hasProjChanged)
        {
            projectionM.setIdentity();
            float aspect = float(vpWidth)/float(vpHeight);
            float theta = fovY*0.5f;
            float range = farDist - nearDist;
            float invtan = 1.0f/tan(theta);
            projectionM(0,0) = invtan / aspect;
            projectionM(1,1) = invtan;
            projectionM(2,2) = -(nearDist + farDist) / range;
            projectionM(3,2) = -1;
            projectionM(2,3) = -2 * nearDist * farDist / range;
            projectionM(3,3) = 0;
            hasProjChanged = false;
        }
        return projectionM;
    }

    void Camera::setDirection(const vec3f &newDirection)
    {
        vec3f _up = up();
//        cout << "du " << _up << endl;
        mat3f axes;
        axes.col(2) = (-newDirection).normalized();
        axes.col(0) = _up.cross(axes.col(2)).normalized();
        axes.col(1) = axes.col(2).cross(axes.col(0)).normalized();
        setOrientation(Eigen::Quaternionf(axes));
//        cout << "do " << orientation.matrix() << endl;
        hasViewChanged = true;
    }

    vec3f Camera::getTarget() { return target; }

    void Camera::setTarget(const vec3f &t)
    {
        target = t;
        if (!target.isApprox(position))
        {
            vec3f dir = target - position;
            setDirection(dir.normalized());
        }
    }

    void Camera::zoom(float distance)
    {
        float d = (position - target).norm();
        if(d > distance)
        {
            setPosition(position + direction() * distance);
            hasViewChanged = true;
        }
    }

    // TODO: switch camera to using a physics/velocity model
    // so that interactions are consistent.
    // the current method could be unstable to framerate
    // and event processing speeds.
    void Camera::handleKeyEvent(window::KeyEvent e) {
        if (e.action == window::action::PRESSED || e.action == window::action::REPEAT)
        {
            switch (e.key)
            {
                default:
                    break;

                // zoom = move forward/back
                case window::key::W:
                    zoom(0.2f);
                    break;
                case window::key::S:
                    zoom(-0.2f);
                    break;

                // move (slide)
                case window::key::C:
                    localTranslate({0, -0.2f, 0});
                    break;
                case window::key::E:
                    localTranslate({0, 0.2f, 0});
                    break;
                case window::key::A:
                    localTranslate({-0.2f,0,0});
                    break;
                case window::key::D:
                    localTranslate({0.2f,0,0});
                    break;

                // look around
                case window::key::I:
                    if (e.mods == window::mods::SHIFT)
                        rotateAroundTarget(quat4f(Eigen::AngleAxisf(0.1f, vec3f::UnitX())));
                    else localRotate(quat4f(Eigen::AngleAxisf(0.1f, vec3f::UnitX())));
                    break;
                case window::key::J:
                    if (e.mods == window::mods::SHIFT)
                        rotateAroundTarget(quat4f(Eigen::AngleAxisf(0.1f, vec3f::UnitY())));
                    else localRotate(quat4f(Eigen::AngleAxisf(0.1f, vec3f::UnitY())));
                    break;
                case window::key::K:
                    if (e.mods == window::mods::SHIFT)
                        rotateAroundTarget(quat4f(Eigen::AngleAxisf(-0.1f, vec3f::UnitX())));
                    else localRotate(quat4f(Eigen::AngleAxisf(-0.1f, vec3f::UnitX())));
                    break;
                case window::key::L:
                    if (e.mods == window::mods::SHIFT)
                        rotateAroundTarget(quat4f(Eigen::AngleAxisf(-0.1f, vec3f::UnitY())));
                    else localRotate(quat4f(Eigen::AngleAxisf(-0.1f, vec3f::UnitY())));
                    break;

                // rolling
                case window::key::U:
                    localRotate(quat4f(Eigen::AngleAxisf(0.1f, vec3f::UnitZ())));
                    break;
                case window::key::O:
                    localRotate(quat4f(Eigen::AngleAxisf(-0.1f, vec3f::UnitZ())));
                    break;

            }
        }
    }
    void Camera::handleMouseButtonEvent(window::MouseButtonEvent e) {
        if (e.action == window::action::PRESSED && e.button == window::button::LEFT)
        {
            if (!dragging)
            {
                dragPosition = e.location;
                dragging = true;
            }
        }
        if (e.action == window::action::RELEASED)
        {
            dragging = false;
        }
    }
    void Camera::handleCursorMoveEvent(window::CursorMoveEvent e) {
        if (dragging)
        {
            vec2f offset = (dragPosition - e.location).array() * dragSensitivity.array();
            dragPosition = e.location;

            rotateAroundTarget(quat4f(
                    Eigen::AngleAxisf(-offset.y(), vec3f::UnitX())
                    * Eigen::AngleAxisf(-offset.x(), vec3f::UnitY())
            ));
        }
    }

    void Camera::handleScrollEvent(window::ScrollEvent e) {
        if (!dragging)
        {
            zoom(static_cast<float>(e.yOffset * scrollSensitivity.y()));
            localTranslate({e.xOffset * scrollSensitivity.x(),0,0});
        }
    }

    void Camera::rotateAroundTarget(const quat4f &q) {
        mat4f mrot, mt, mtm;

        // force update of transform matrix
        aff3f vm = viewMatrix();

        vec3f t = viewM * target;
        viewM = Eigen::Translation3f(t) * q * Eigen::Translation3f(-t) * viewM;

        quat4f qa(viewM.linear());
        qa = qa.conjugate();
        setOrientation(qa);
        setPosition(- (qa * viewM.translation()));

        hasViewChanged = true;
    }

    void Camera::localTranslate(const vec3f &t) {
        vec3f trans = orientation * t;
        setPosition(position + trans);
        setTarget(target + trans);
        hasViewChanged = true;
    }

    void Camera::localRotate(const quat4f &r) {
        float dist = (position - target).norm();
        setOrientation(orientation * r);
        target = position + dist * direction();
        hasViewChanged = true;
    }

    void Camera::reset(const vec3f &pos, const vec3f &t) {
        setPosition(pos);
        setTarget(t);
        setOrientation({0,0,1,0});
        hasViewChanged = true;
    }
}
