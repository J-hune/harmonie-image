#pragma once
#include "src/images/imageLoader.h"
#include <vector>
#include <array>
#include <algorithm>
#include <map>
#include <cfloat>


inline float fposmod(float a, float b){
    a = fmod(a, b);
    return (a >= 0 )? a : a + b; 
}

float angle_distance(float a, float b){
    a = fposmod(a, 1.0);
    b = fposmod(b, 1.0);
    return std::min(abs(a-b), abs(a + 1.0f - b));
}


bool between(float v, float a, float b){
    a = fposmod(a, 1.0);
    b = fposmod(b, 1.0);
    v = fposmod(v, 1.0);
    if (b > a) return a <= v && v <= b;
    return (v >= a) || (v <= b);
}

struct HarmonicTemplate{
    std::vector<std::array<float, 2> > sectors;

    HarmonicTemplate() = default;

    HarmonicTemplate(std::vector<std::array<float, 2> > s): sectors(s) {
        for (auto& v: sectors){ // convet to [0, 1] here
            v[0] /= 360.0;
            v[1] /= 360.0;
        }
    };
};

double E(HarmonicTemplate templ, float alpha, Vec3 p){

    float best_hue;
    double best_dist = DBL_MAX;
    double dist;
    
    for (auto & sector: templ.sectors){
        float min_bound = fposmod(sector[1] + alpha - sector[0] / 2.0, 1.0);
        float max_bound = fposmod(sector[1] + alpha + sector[0] / 2.0, 1.0);

        float hue = p[0];

        //printf("%f,  [%f, %f] \n", hue, min_bound, max_bound);

        if (between(hue, min_bound, max_bound)) return hue;

        dist = angle_distance(hue, min_bound);
        if (dist < best_dist){
            best_dist = dist;
            best_hue = min_bound;
        }

        dist = angle_distance(hue, max_bound);
        if (dist < best_dist){
            best_dist = dist;
            best_hue = max_bound;
        }
    }

    //printf("finale: ", best_hue);
    return best_hue;
    
}


float arc_length(float hue){
    return 2.0 * M_PI * hue;
}


// "distance" function
// X is an image
// (templates, alpha) is an harmonic scheme: a templates offset by an angle

double F(const ImageRGB&  X, HarmonicTemplate templ, float alpha){
    double acc = 0;

    for (int   i = 0; i < X.w; ++i){
        for (int   j = 0; j < X.h; ++j){
            auto p = X(i, j);

            float closest_hue = E(templ, alpha, p);
            acc += arc_length(abs(p[0] - closest_hue)) * p[1];
        }
    }
    return acc;
}


map<string, HarmonicTemplate> templates = {
    {"i", HarmonicTemplate({{18.0, 0}})},
    {"V", HarmonicTemplate({{93.6, 0}})},
    {"L", HarmonicTemplate({{18.0, 0}, {72.2, 90.0}})},
    {"I", HarmonicTemplate({{18.0, 0}, {18.0, 180.0}})},
    {"T", HarmonicTemplate({{180.0, 0}})},
    {"Y", HarmonicTemplate({{93.6, 0}, {18.0, 180.0}})},
    {"X", HarmonicTemplate({{93.6, 0}, {93.6, 180.0}})}
};


constexpr int delta_angle = 10;

float findMinimizingAngle(const ImageRGB & X, const HarmonicTemplate & templ, double* best_dist = nullptr){

    // TODO multithread from here
    double best_distance_for_angle = DBL_MAX;
    HarmonicTemplate best_template;
    float best_angle;
    for (int   alpha = 0; alpha < 360; alpha += delta_angle){
        double dist = F(X, templ, alpha / 360.0f);

        if (dist < best_distance_for_angle){
            best_distance_for_angle = dist;
            best_angle = alpha / 360.0f;
        }
    }
    if (best_dist) *best_dist = best_distance_for_angle;
    return best_angle;
}


void findMinimizingScheme(const ImageRGB & X, HarmonicTemplate & return_template, float & return_angle){
    
    double best_distance = DBL_MAX;
    for (const auto& [key, value] : templates) {
        printf("processing template %s \n", key.c_str());

        // TODO multithread from here
        double best_distance_for_angle = DBL_MAX;
        float best_angle = findMinimizingAngle(X, value, &best_distance_for_angle);

        if (best_distance_for_angle < best_distance){
            best_distance = best_distance_for_angle;
            return_template = value;
            return_angle = best_angle;
        }
    }
}

void projectImageOnScheme(ImageRGB & X, HarmonicTemplate & templ, float angle){
    for (int   i = 0; i < X.w; ++i){
        for (int   j = 0; j < X.h; ++j){

            X(i, j)[0] = E(templ, angle, X(i, j));
        }
    }
}
