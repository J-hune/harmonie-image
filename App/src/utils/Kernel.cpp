#include "Kernel.h"
#include <cmath>

void Kernel::saveDistribution(std::string name) const{

    const char * nom_image = name.c_str();
    FILE *f_data;

    if( (f_data = fopen(nom_image, "wb")) == NULL){
        printf("\n[Export Histogramme] Pas d'acces en ecriture sur le fichier %s \n", nom_image);
        exit(EXIT_FAILURE);
    }

    for (int   i = -extent; i <= extent; ++i){

        fprintf(f_data,
            "%d %f\n",
            i,
            (float)(*this)(0, i)
        );
    }
    fclose(f_data);
}


Kernel KernelUtilities::gaussian(int extent, double smoothness){
    double sigma = (extent * 2.0 + 1.0) / (6.0 * std::max(0.001, smoothness));

    Kernel res(extent);

    for (int   x = -extent;x <= extent ; ++x){
        for (int   y = -extent;y <= extent ; ++y){

            int sqrdist = x * x + y * y;
            double c = 2.0 * sigma * sigma;


            res(x, y) = exp(-sqrdist / c) * (1.0/ (2.0 * M_PI * sigma * sigma));
        }
    }
    res.normalize();
    return res;
}