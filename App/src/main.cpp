#include "src/images/imageLoader.h"
#include <string>
#include <iostream>

#include "./src/algos/indicators.h"

#include "./src/utils/Kernel.h"


int main(void){
    
    const string input_path = "./img/edgar.ppm";
    const string output_path = "./img_out/";
    
    // small example
    ImageRGB base = ImageRGB::fromPPM(input_path);

    ImageRGB base_hsv = base.convertTo(PixelType::HSV);
    
    base_hsv = base_hsv.apply(
        [](const Vec3 & col) -> Vec3 {
            return Vec3(col[0]+0.4, col[1], col[2]);
        }
    );
    // reconversion en rgb
    base_hsv.convertTo(PixelType::RGB).saveAs(output_path + "test1.ppm");
    
    return 0;
}