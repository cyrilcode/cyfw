
#ifndef CYFW_TEXTURE_H
#define CYFW_TEXTURE_H


#include "types.h"
#include "VideoGrabber.h"

namespace cy
{
    class Texture {
    public:
        int set(int w, int h);
        void update();
        void copyPixels(ca::PixelBuffer&);

    private:
        unsigned int texId;
    };


}


#endif
