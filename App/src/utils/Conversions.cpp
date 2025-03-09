#include "Vec3.h"
#include <algorithm>

Vec3 toYCrCb(const Vec3 & base){
    float y = base.luminance();
    return Vec3(
        y,
        0.5f * base[0] - 0.4187f * base[1] - 0.0813f * base[2],
        -0.1687f * base[0] - 0.3313f * base[1] + 0.5f * base[2]
    );

}


Vec3 fromYCrCb(const Vec3 & base){
    return Vec3(
            base[0] + 1.402f * base[1], 
            base[0] - 0.34414f*base[2] - 0.714414f*base[1],
            base[0] + 1.772f *base[2]
    );
}


//from https://www.rapidtables.com/convert/color/rgb-to-hsv.html
constexpr float deg_60 = 1.0 / 6.0;
Vec3 toHSV(const Vec3 & base){

    float h, s, v;

    float max = base[0] > base[2] ? (base[0] > base[1] ? base[0] : base[1]) : (base[2] > base[1] ? base[2] : base[1]);
    float min = base[0] < base[2] ? (base[0] < base[1] ? base[0] : base[1]) : (base[2] < base[1] ? base[2] : base[1]);
    float delta = max-min;

    //hue
    if (delta == 0){

        h = 0.0;
    } else if (max == base[0]){
        h = deg_60 * fmod((base[1]-base[2])/ delta, 6.0f);

    } else if (max == base[1]){
        h = deg_60 *( (base[2]-base[0]) / delta + 2.0);

    } else if (max == base[2]){
        h = deg_60 * ((base[0]-base[1])/ delta + 4.0);

    }
    //value
    v = max;

    //saturation
    s = (max == 0)? 0: delta / v;

    return Vec3(h, s, v);

};

// from https://www.rapidtables.com/convert/color/hsv-to-rgb.html
Vec3 fromHSV(const Vec3 & base){

    Vec3 res;
    
    float h = fmod(base[0], 1.0);
    float s = base[1];
    float v = base[2];

    if (h < 0) h = h+1.0;

    float C, X, m;


    C = v * s;

    X = C * (1.0 - fabs(
            fmod(h / deg_60, 2.0) - 1.0
        )
    );

    m = v-C;

    if (h < 1.0 * deg_60){

        res = Vec3(C, X, 0);

    } else if (h < 2.0 * deg_60){

        res = Vec3(X, C, 0);

    } else if (h < 3.0 * deg_60){

        res = Vec3(0, C, X);

    } else if (h < 4.0 * deg_60){

        res = Vec3(0, X, C);

    } else if (h < 5.0 * deg_60){

        res = Vec3(X, 0, C);

    } else{

        res = Vec3(C, 0, X);
    }

    return res + Vec3(m, m, m);
}




Vec3 Vec3::convertFrom(PixelType t) const{

    switch (t){
        case PixelType::RGB:
            return *this;
        case PixelType::YCRCB:
            return fromYCrCb(*this);
        case PixelType::HSV:
            return fromHSV(*this);
    }
    return *this;
}

Vec3 Vec3::convertTo(PixelType t) const{
    switch (t){
        case PixelType::RGB:
            return *this;
        case PixelType::YCRCB:
            return toYCrCb(*this);
        case PixelType::HSV:
            return toHSV(*this);
    }
    return *this;
}


