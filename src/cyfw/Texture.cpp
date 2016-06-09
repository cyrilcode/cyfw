
#include "Texture.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
using namespace std;

namespace cy
{

    int Texture::set(int w, int h) {
        return -1;
    }

    void Texture::update() {

    }

    void Texture::copyPixels(ca::PixelBuffer &b) {
//        memcpy(pixels, b.pixels, b.nbytes);
    }
}