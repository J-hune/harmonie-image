#pragma once

#include <algorithm>
#include <vector>
#include "src/utils/Vec3.h"
#include <iostream>





class Color{
public:
    unsigned char r = 0;
    unsigned char g = 0;
    unsigned char b = 0;

    Color() = default;
    Color(unsigned char red, unsigned char green, unsigned char blue): r(red), g(green), b(blue) {}
    Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha): r(red), g(green), b(blue) {}
    Color(int red, int green, int blue): r(red), g(green), b(blue) {}
    Color(unsigned char v){
        r = g = b = v;
    }

    Color(const Vec3 & v){

        r = (unsigned char)(std::clamp(v[0] * 255, 0.0f, 255.0f));
        g = (unsigned char)(std::clamp(v[1] * 255, 0.0f, 255.0f));
        b = (unsigned char)(std::clamp(v[2] * 255, 0.0f, 255.0f));
    }
    Color(float a, float b, float c){

        r = (unsigned char)(std::clamp(a * 255, 0.0f, 255.0f));
        g = (unsigned char)(std::clamp(b * 255, 0.0f, 255.0f));
        b = (unsigned char)(std::clamp(c * 255, 0.0f, 255.0f));
    }

    inline Vec3 toVec3() const {
        return Vec3(float(r)/255.0, float(g)/255.0, float(b)/255.0);
    }

    Color& operator += (const Color& rhs){
        r = std::clamp(r+rhs.r, 0, 255);
        g = std::clamp(g+rhs.g, 0, 255);
        b = std::clamp(b+rhs.b, 0, 255);
        return *this;
    }

    Color& operator -= (const Color& rhs){
        r = std::clamp(r-rhs.r, 0, 255);
        g = std::clamp(g-rhs.g, 0, 255);
        b = std::clamp(b-rhs.b, 0, 255);
        return *this;
    }

    template<typename T> // dirty or genius? I don't know. I never know.
    Color& operator /= (const T& rhs){
        r = std::clamp(r/ static_cast<unsigned int> (rhs), 0u, 255u);
        g = std::clamp(g/ static_cast<unsigned int> (rhs), 0u, 255u);
        b = std::clamp(b/ static_cast<unsigned int> (rhs), 0u, 255u);
        return *this;
    }

    template<typename T>
    Color& operator *= (const T& rhs){
        r = std::clamp(r* static_cast<unsigned int> (rhs), 0u, 255u);
        g = std::clamp(g* static_cast<unsigned int> (rhs), 0u, 255u);
        b = std::clamp(b* static_cast<unsigned int> (rhs), 0u, 255u);
        return *this;
    }

    unsigned char & operator [](int i){
        return (i == 0)? r : (i==1)? g: b;
    }
    const unsigned char & operator [](int i) const { 
        return (i == 0)? r : (i==1)? g: b;
    }

    unsigned char luminance() const {
        return std::clamp(static_cast<unsigned int> (0.299*r + 0.587*g + 0.114*b), 0u, 255u);
    }
    unsigned char mean() const {
        return 0.3333333*r + 0.3333333*g + 0.3333333*b;
    }

    float distance_euclidian_squared(const Color& other) const{
        Vec3 v = (*this).toVec3() - other.toVec3();
        return v.squareLength()*255;
    }

    float distance_euclidian(const Color& other) const {
        Vec3 v = (*this).toVec3() - other.toVec3();
        return v.length()*255;
    }

    void quantize_to_palette(const std::vector < Color > & p){
        Color candidate;
        float dist = 999999;

        for (int i = 0; i < p.size(); ++i){

            float new_dist = distance_euclidian_squared(p[i]);

            if (new_dist < dist){
                dist = new_dist;
                candidate = p[i];
            }
        }
        r = candidate.r;
        g = candidate.g;
        b = candidate.b;
    }

    //Stored in a color: be careful!
    Color toYCrCb(){
        unsigned char y = luminance();
        return Color(
            (unsigned char)y,
            (unsigned char)std::clamp((
                (0.5f * (float)r - 0.4187f * (float)g - 0.0813f * (float)b) + 128.0f), 
                0.0f, 255.0f
                ),
            (unsigned char)std::clamp((
                (-0.1687f * (float)r - 0.3313f * (float)g + 0.5f * b) + 128.0f), 
                0.0f, 255.0f
                )
        );

    }


    Color fromYCrCb(){
        return Color(
            (unsigned char)std::clamp((
                (float)r + 1.402f * ((float)g - 128.0f)), 
                0.0f, 255.0f
                ),
            (unsigned char)std::clamp((
                (float)r - 0.34414f*((float)b-128.0f) - 0.714414f*((float)g - 128.0f)), 
                0.0f, 255.0f
                ),
            (unsigned char)std::clamp((
                (float)r + 1.772f *((float)b - 128.0f)), 
                0.0f, 255.0f
                )
        );
    }
    /*
    //from https://mattlockyer.github.io/iat455/documents/rgb-Vec3.pdf
    Vec3 to_Hsv(){
    
        float r = r/255.0;
        float g = g/255.0;
        float b = b/255.0;

        float h, s, v;

        float max = r > b ? (r > g ? r : g) : (b > g ? b : g);
        float min = r < b ? (r < g ? r : g) : (b < g ? b : g);
        float delta = max-min;

        //hue
        if (delta == 0){

            h = 0.0;
        } else if (max == r){
            h = (g-b)/ delta;

        } else if (max == g){
            h = (b-r) / delta + 2;

        } else if (max == b){
            h = (r-g)/ delta + 4;

        }

        //value
        v = max;

        //saturation
        v == 0 ? s = 0 : s = delta / v;

        return Color(h, s, v);

    };

    static Color from_Vec3(Vec3 soruce){

        float h = fmod(source.h, 6.0);
        float s = source.s;
        float v = source.v;

        if (h < 0) h = h+6.0;

        float a, b, c; //channels


        a = v * (1.0-s);

        b = v * (1- (h - floor(h)) * s);

        c = v * (1- (1- (h-floor(h)))*s);

        Vec3 res; //not a real Vec3

        if (h < 1.0){

            res = {v, c, a};

        } else if (h < 2.0){

            res = {b, v, a};

        } else if (h < 3.0){

            res = {a, v, c};

        } else if (h < 4.0){

            res = {a, b, v};

        } else if (h < 5.0){

            res = {c, a, v};

        } else if (h <= 6.0){

            res = {v, a, b};

        }

        return Color(
            (int)(res.h*255),
            (int)(res.s*255),
            (int)(res.v*255)
        );
    }

    */
};

static inline std::ostream & operator << (std::ostream & s , Color const & p) {
    s << (int)p[0] << "," << (int)p[1] << "," << (int)p[2];
    return s;
}
