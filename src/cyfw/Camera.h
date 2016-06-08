
#ifndef CYFW_CAMERA_H
#define CYFW_CAMERA_H

#include "types.h"
#include "Window.h"

namespace cy
{

    class Camera
    {
    public:
        Camera();
        void setViewport(unsigned int x, unsigned int y, unsigned int w, unsigned int h);
        void setViewport(unsigned int w, unsigned int h);
        void setViewport(vec2f dim);
        void setPosition(const vec3f &pos);
        void setTarget(const vec3f &target);
        void setDirection(const vec3f &newDirection);
        void setOrientation(const quat4f &q);
        quat4f getOrientation();
        vec3f getPosition();
        vec3f getTarget();
        vec3f up() const;
        vec3f right() const;
        vec3f direction() const;
        void zoom(float distance);
        void localTranslate(const vec3f &t);
        void localRotate(const quat4f &r);
        void rotateAroundTarget(const quat4f &q);
        void reset(const vec3f &pos);


        const aff3f& viewMatrix();
        const mat4f& projectionMatrix();

        void handleKeyEvent(window::KeyEvent e);
        void handleScrollEvent(window::ScrollEvent e);
        void handleMouseButtonEvent(window::MouseButtonEvent e);
        void handleCursorMoveEvent(window::CursorMoveEvent e);

    private:
        float fovY;
        float nearDist;
        float farDist;
        unsigned int vpX, vpY, vpWidth, vpHeight;
        vec3f target;
        aff3f viewM;
        mat4f projectionM;
        bool hasViewChanged;
        bool hasProjChanged;
        vec2f scrollSensitivity;
        vec2f dragSensitivity;
        vec2f dragPosition;
        bool dragging;

        vec3f position;
        quat4f orientation;
    };

}



#endif