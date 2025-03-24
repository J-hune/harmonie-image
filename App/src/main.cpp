#include "src/images/imageLoader.h"
#include <string>
#include <iostream>

#include "./src/algos/indicators.h"

#include "./src/utils/Kernel.h"


int main(void){
    
    const string input_path = "./img/0.jpg";
    const string output_path = "./img_out/";
    
    // small example
    ImageRGB base = ImageRGB::from(input_path);

    ImageRGB base_hsv = base.convertTo(PixelType::HSV);
    
    base_hsv = base_hsv.apply(
        [](const Vec3 & col) -> Vec3 {
            return Vec3(fmod(col[0]+0.5, 1.0), col[1], col[2]);
        }
    );
    // reconversion en rgb
    base.saveAs(output_path + "test1.png");
    base.saveHue(output_path + "base.hdat");
    base_hsv = base_hsv.convertTo(PixelType::RGB);
    base_hsv.saveAs(output_path + "test1.jpg");
    base_hsv.saveHue(output_path + "base_offset.hdat");



    std::cout << "[";
    for (int i = 0; i < 255; ++i){
        std::cout << "(" << Vec3(i/255.0, 0.8, 0.8).convertFrom(PixelType::HSV) << "), ";
    }
    std::cout << "]" << std::endl;
    
    return 0;
}