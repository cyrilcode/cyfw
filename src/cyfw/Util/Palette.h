
#ifndef CYFW_H
#define CYFW_H

#include "cyfw/main.h"

namespace cy
{
    // dependency injection static polymorphism
    template<class GENERATOR = ListPalette>
    class Palette {
        GENERATOR p;
    public:
        Palette(GENERATOR generator) : p(generator) {}
        inline color lerp(float f) { p.lerp(f); }
        inline color select(float f) { p.select(f); }
    };

    class ListPalette
    {
        float total;
        // a palette list is a vector of pairs
        // where the first of pair is color and
        // second of pair is distance from start
        // of palette.
        std::vector<std::pair<color,float>> colors;
    public:
        color lerp(float f); // interpolate colors based on distance from f
        color select(float f); // select color nearest to f
        inline void addColor(color c, float d) { total+=d; colors.push_back(std::make_pair(c, d)); }
    };

    // Procedural palette generation with cosine
    // c.f. http://www.iquilezles.org/www/articles/palettes/palettes.htm
    class ProceduralPalette
    {
        vec3f a,b,c,d;
    public:
        ProceduralPalette(vec3f a_, vec3f b_, vec3f c_, vec3f d_)
                : a(a_), b(b_), c(c_), d(d_) {}
        inline color lerp(float f) {
            return a.array() + b.array() * (math::TWO_PI * (c.array() * f + d.array())).cos();
        }
        inline select(float f) { return lerp(f); }
    };
}


#endif
