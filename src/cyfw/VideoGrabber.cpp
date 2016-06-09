
#include "main.h"

#include <iostream>
using namespace std;
using namespace ca;

namespace cy
{
//    ptr<VideoGrabber> VideoGrabber::create() {
//        return make_shared<VideoGrabber>();
//    }
//
//    void VideoGrabber::setDevice(int d) {
//        device = d;
//    }
//
//    VideoGrabber::~VideoGrabber()
//    {
//        if(cap.stop() < 0) {
//            cerr << "VideoGrabber Error: could not stop device" << endl;
//        }
//        if(cap.close() < 0) {
//            cerr << "VideoGrabber Error: could not close device" << endl;
//        }
//    }
//
//    void VideoGrabber::listDevices() {
//        cap.listDevices();
//    }
//
//    int VideoGrabber::open(int width, int height) {
//        return cap.open(device, width, height);
//    }
//
//
//    void VideoGrabber::draw(int x, int y, int w, int h)
//    {
//        cap.draw(x, y, w, h);
//    }
//
//    void VideoGrabber::resize(int w, int h) {
//        cap.resize(w, h);
//    }
}


#define VIDEO_CAPTURE_IMPLEMENTATION
#include <videocapture/CaptureGL.h>
