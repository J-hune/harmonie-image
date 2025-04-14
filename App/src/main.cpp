#include "src/images/imageLoader.h"
#include <string>
#include <iostream>
#include <chrono>

#include "./src/algos/cohen-or.hpp"

int main(int argc, char *argv[]){
    
    // the arguments are :
    // - image path
    // - templace to use or "ND"
    //  - angle to use or "ND"
    bool autodetectTemplate = true;
    bool autodetectAngle = true;

    string input_path = string(argv[1]);

    string input_template = string(argv[2]);
    string input_angle = string(argv[3]);

    const string output_path = "./";

    
    std::cout << input_path << std::endl;
    // small example
    ImageRGB base = ImageRGB::from(input_path);

    ImageRGB base_hsv = base.convertTo(PixelType::HSV);

    HarmonicTemplate t_best;
    float angle_best;
    if (input_template != "ND"){
        t_best = templates[ input_template ];
        autodetectTemplate = false;
    }

    if (input_angle != "ND"){
        angle_best = std::stof(input_angle) / 360.0f;
        autodetectAngle = false;
    }

    // if we autodetect the template, we must autodetect the angle (would make no sense to set an angle without setting the template aswell)
    if (autodetectTemplate){
        findMinimizingScheme(base_hsv, t_best, angle_best);
    }
    else if (autodetectAngle){
        angle_best = findMinimizingAngle(base_hsv, t_best);
    }

    std::cout << "\nusing template " << input_template << std::endl;
    std::cout << "using angle " << angle_best << std::endl;
    projectImageOnScheme(base_hsv, t_best, angle_best);

    // reconversion en rgb
    base_hsv.convertTo(PixelType::RGB).saveAs(output_path + "output.png");
    return 0;
}