#include "src/images/imageLoader.h"
#include <string>
#include <iostream>
#include <chrono>

#include "./src/algos/cohen-or.hpp"

int main(void){
    
    const string input_path = "./img/nenuphars.png";
    const string output_path = "./img_out/";
    
    // small example
    ImageRGB base = ImageRGB::from(input_path);

    ImageRGB base_hsv = base.convertTo(PixelType::HSV);

    HarmonicTemplate t_best = templates["V"];
    float angle_best = 10.0;

    auto start = std::chrono::steady_clock::now();

    //findMinimizingScheme(base_hsv, t_best, angle_best);

    angle_best = findMinimizingAngle(base_hsv, templates["I"]);
    
    for (auto & s : t_best.sectors){
        std::cout << "sec " << s[0] << " " << s[1] << std::endl;
    }

    std::cout << "angle " << angle_best << std::endl;
    


    projectImageOnScheme(base_hsv, t_best, angle_best);


    auto end = std::chrono::steady_clock::now();
    
    std::cout << "Temps de traitement : " <<std::chrono::duration<double, std::milli>(end - start).count()/1000.0 << "s"<< std::endl;

    // reconversion en rgb
    base_hsv.convertTo(PixelType::RGB).saveAs(output_path + "nenu.png");
    base_hsv.convertTo(PixelType::RGB).compareTo(base, 5.0).saveAs(output_path + "diff.png");

    return 0;
}