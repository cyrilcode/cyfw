

#ifndef CYFW_COLOR_H
#define CYFW_COLOR_H

namespace cy
{


    class color
    {
        vec4f _v;
    public:
//        inline color(vec4f value) : _v(value) {}
        inline color(float r, float g, float b, float a) : _v(r, g, b, a) {}
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

        // interpolate 2 colours based on value of f
        // eg, f=0 then c1, f=1 then c2, f=0.5 50:50 mix of both
        static color lerp(color c1, color c2, float f);
        static color hex(int hexValue); // generate from hex value
        static color hsb(float hue, float saturation, float brightness, float alpha);

        // TODO: add all named colors
        // c.f. https://www.w3.org/TR/css3-color/
        constexpr static int red = 0xFF0000;

    };


}



#endif //CYFW_COLOR_H
