#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "src/utils/Color.h"
#include <array>
#include "./src/utils/Kernel.h"
#include <functional>
#include "stb_image.h"
#include "stb_image_write.h"


// Based on http://josiahmanson.com/prose/optimize_ppm/

using namespace std;
using Histogram = std::array< std::array<unsigned int, 3>, 256>;
using HistogramProbas = std::array< std::array<double, 3>, 256>;
void eat_comment(ifstream &f);



class ImageRGB;
class ImageByte
{
    friend void load_ppm(ImageByte &img, const string &name);

protected:
    vector< Color > data;
public:
    int w, h;

    Color & operator() (int u, int v){
        return data.at(v * w + u);
    }

    Color operator() (int u, int v) const{
        return data.at(v * w + u);
    }
    ImageByte() = default;
    ImageByte(int u, int v){
        w = u;
        h = v;
        data.resize(u * v);
    }

    static ImageByte fromPPM(const string &name);


    void saveAs(string name, float quality) const;

    Histogram getHistogramData(bool as_grey = false) const;

    HistogramProbas getHistogramProba(bool as_grey = false) const;

    HistogramProbas getHistogramProbaRepartition(bool as_grey = false) const;

    void saveHistogramComparison(ImageByte other, string name, bool as_grey) const;

    void saveHistogramData(string name, bool as_grey = false, bool cumulative = false) const;

    void saveHistogramProbabilisticData(string name, bool as_grey = false) const;

    const vector< Color > & getData(){
        return data;
    } 

    ImageByte toGreyscaleLum() const {
        ImageByte res(w, h);

        for (int   i =0; i < w; ++i){
            for (int   j =0; j < h; ++j){
                res(i, j) = Color((*this)(i,j).luminance());
            }
        }

        return res;
    }

    ImageByte compareTo(const ImageByte & other, float fac = 1.0) const {
        ImageByte res(w, h);

        for (int   i =0; i < w; ++i){
            for (int   j =0; j < h; ++j){

                Vec3 diff = (*this)(i,j).toVec3() - other(i,j).toVec3();
                res(i, j) = Color(diff * fac + Vec3(0.5));
            }
        }
        return res;
    }
    enum RepeatMode {TILE, EXTEND, MIRROR};
    Color getPixelSafe(int u, int v, RepeatMode repeat_mode = RepeatMode::MIRROR) const;

    ImageByte convolve(const Kernel & kernel, RepeatMode repeat_mode = RepeatMode::MIRROR) const;

    ImageByte mask_mix(const ImageByte & other, const ImageRGB & mask) const;
};

enum loadedFormat {
    rgb,
    rbg
};

class ImageRGB
{
    friend void load_ppm(ImageByte &img, const string &name);

protected:
    vector< Vec3 > data;

    PixelType type = PixelType::RGB;

public:
    int w, h;

    Vec3 & operator() (int u, int v){
        return data[v * w + u];
    }

    Vec3 operator() (int u, int v) const{
        return data[v * w + u];
    }
    ImageRGB() = default;
    ImageRGB(int u, int v){
        w = u;
        h = v;
        data.resize(u * v);
    }

    ImageRGB(int u, int v, PixelType t){
        w = u;
        h = v;
        data.resize(u * v);
        type = t;
    }

    ImageRGB(ImageByte img){
        w = img.w;
        h = img.h;
        data.resize(w*h);
        
        for (int   i = 0; i < w*h; ++i) data[i] = img.getData()[i].toVec3();
    }

    PixelType getType() const { return type; }

    static ImageRGB fromPPM(const string &name);

    static ImageRGB from(const string &name){
        
        int w, h;
        int comp = -1;

        unsigned char* image = stbi_load(name.c_str(), &w, &h, &comp, 3);

        ImageRGB res(w, h);

        if (comp == 1){
            for (int   i = 0; i < w * h * comp; i+=comp){
                res.data[i] = Vec3(image[i]/255.0);
            }
        }
        else if (comp == 3 || comp == 4){
            for (int   i = 0; i < w * h; i+=1){
                res.data[i] = Vec3(image[3*i]/255.0, image[3*i+1]/255.0, image[3*i+2]/255.0);
            }
        }
        else {
            std::cout << "[IMAGE LOADING] number of components (" << comp << ") is not 1, 3 or 4." << std::endl;
            exit(1);
        }

        stbi_image_free(image);

        return res;
    }

    ImageByte toImage(bool has_neg = true) const;

    void saveAs(string name, float quality=1.0, bool has_neg = false) const{
        toImage(has_neg).saveAs(name, quality);
    }

    Histogram getHistogramData(bool as_grey = false) const;

    HistogramProbas getHistogramProba(bool as_grey = false) const;

    HistogramProbas getHistogramProbaRepartition(bool as_grey = false) const;

    void saveHistogramComparison(ImageByte other, string name, bool as_grey) const;

    void saveHistogramData(string name, bool as_grey = false, bool cumulative = false) const;

    void saveHistogramProbabilisticData(string name, bool as_grey = false) const;

    void saveHue(string name) const{
        auto Img = convertTo(PixelType::HSV);
        Img.apply([](const Vec3 & v){
            return Vec3(v[0]);
        }
        ).saveHistogramData(name);
    }


    const vector< Vec3 > & getData(){
        return data;
    }

    enum RepeatMode {TILE, EXTEND, MIRROR};
    Vec3 getPixelSafe(int u, int v, RepeatMode repeat_mode = RepeatMode::MIRROR) const;

    ImageRGB convolve(const Kernel & kernel, RepeatMode repeat_mode = RepeatMode::MIRROR) const;

    ImageRGB compareTo(const ImageRGB & other, float fac = 1.0) const {
        ImageRGB res(w, h);

        for (int   i =0; i < w; ++i){
            for (int   j =0; j < h; ++j){

                Vec3 diff = (*this)(i,j) - other(i,j);
                res(i, j) = diff * fac;
            }
        }
        return res;
    }

    ImageRGB convertTo(PixelType t) const {
        ImageRGB res(w, h);

        for (int   i =0; i < w; ++i){
            for (int   j =0; j < h; ++j){
                res(i, j) = ((*this)(i,j)).convert(type, t);
            }
        }
        res.type = t;
        return res;
    }

    ImageRGB apply(std::function<Vec3(const Vec3 &)> f){
        ImageRGB res(w, h, type);

        for (int   i =0; i < w; ++i){
            for (int   j =0; j < h; ++j){
                res(i, j) = f( (*this)(i, j) );
            }
        }
        return res;
    }

    ImageRGB apply(std::function<Vec3(const ImageRGB &, int u, int v)> f){
        ImageRGB res(w, h, type);

        for (int   i =0; i < w; ++i){
            for (int   j =0; j < h; ++j){
                res(i, j) = f( (*this), i, j );
            }
        }
        return res;
    }
};
