
#ifndef CYFW_VIDEOGRABBER_H
#define CYFW_VIDEOGRABBER_H

#include "types.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <videocapture/CaptureGL.h>

namespace cy
{
    typedef ca::CaptureGL VideoGrabber;

//    class VideoGrabber {
//    public:
//        ~VideoGrabber();
//        void listDevices();
//
//        inline int open(int width, int height);
//        inline int start() { return cap.start(); }
//        inline int stop() { return cap.stop(); }
//        inline int update() { return cap.update(); }
//
//        void setDevice(int);
//        void draw(int x, int y, int w, int h);
//        void resize(int w, int h);
//
//        static ptr<VideoGrabber> create();
//    private:
//        int device;
//        ca::CaptureGL cap;
//        ca::Frame info;
//    };

}


#endif
