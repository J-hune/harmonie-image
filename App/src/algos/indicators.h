#pragma once
#include "src/images/imageLoader.h"
#include <vector>
#include <array>



namespace indicators {

    double eqm ( const ImageByte& raw_img, const ImageByte& output_img){
        double res = 0;
        for (int   i =0; i < raw_img.w; ++i){
            for (int   j = 0; j < raw_img.h; ++j){

                for (int   k = 0; k < 3; ++k){
                    res += (raw_img(i,j)[k] - output_img(i, j)[k]) * (raw_img(i,j)[k] - output_img(i, j)[k]);
                }
            }
        }
        return res / (output_img.h * output_img.w * 3.0);
    }

    double psnr( const ImageByte& raw_img, const ImageByte& output_img){
        return 10.0 * log10((255* 255) / eqm(raw_img, output_img));
    }

    double compressionRatio( const ImageByte& raw_img, const ImageByte& output_img){
        return (raw_img.w * raw_img.h) / (output_img.w * output_img.h);
    }

    double entropy(const ImageByte& img, int channel = 0){
        unsigned int count[256] = {0};
        for (int   u=0; u < img.w; ++u){
            for (int   v=0; v < img.h; ++v){
                const Color & c = img(u, v);
                count[c[channel]] +=1;
            }
        }
        double res = 0.0;
        for (int   u=0; u < 256; ++u){
            double p_alpha = (float)count[u] / (img.w * img.h);
            if (p_alpha > 0.0){
                res += p_alpha * log2(p_alpha);
            }
        }

        return -res;
    }
}