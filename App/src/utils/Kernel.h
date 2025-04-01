#pragma once

#include <vector>
#include <string>
class Kernel{
    std::vector<double> data;

public:
    const int extent;

    Kernel(const std::vector<double>& data, int extent)
        : data(data),
        extent(extent)
        {}

    Kernel(int extent)
        : extent(extent)
        {
            data = std::vector<double>((2 * extent + 1) * (2 * extent + 1));
        }
    double& operator() (int x, int y) {
        return data[(x+extent) + (y+extent) * (2 * extent + 1)];
    }
    double operator() (int x, int y) const {
        return data[(x+extent) + (y+extent) * (2 * extent + 1)];
    }

    void saveDistribution(std::string path) const;

    void normalize(){
        double sum = 0;
        for (size_t  i = 0; i < data.size();++i) sum += data[i];

        sum = 1.0/sum;

        for (size_t  i = 0; i < data.size();++i) data[i] *= sum;
    }
};

namespace KernelUtilities{
    const Kernel Base_3(
        {
            1.0, 1.0, 1.0,
            1.0, 1.0, 1.0,
            1.0, 1.0, 1.0
        },
        1
    );

    const Kernel Base_7(
        {
            1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0
        },
        3
    );

    const Kernel Norm1_7(
        {
             0 ,  0 , 0  , 1.0, 0  , 0  ,  0 ,
            0  , 0  , 1.0, 1.0, 1.0, 0  , 0  ,
            0  , 1.0, 1.0, 1.0, 1.0, 1.0, 0  ,
            1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
            0  , 1.0, 1.0, 1.0, 1.0, 1.0, 0  ,
            0  , 0  , 1.0, 1.0, 1.0, 0  , 0  ,
             0 ,  0 , 0  , 1.0, 0  , 0  ,  0 
        },
        3
    );

    const Kernel test(
        {
             0 ,  1 , 2,
             3, 4, 5,
             6, 7, 8
        },
        1
    );
    const Kernel laplacian_3(
        {
            0.0, 1.0, 0.0,
            1.0, -4.0, 1.0,
            0.0, 1.0, 0.0
        },
        1
    );
    const Kernel laplacian_5(
        {
            0.0, 0.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 0.0,
            1.0, 0.0, -4.0, 0.0, 1.0,
            0.0, 0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0, 0.0
        },
        2
    );

    /*
    https://www.youtube.com/watch?v=kOkfC5fLfgE
    at 29:52
    */

    Kernel gaussian(int extent, double smoothness);
}