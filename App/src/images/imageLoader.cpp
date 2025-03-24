
#include "imageLoader.h"
#include <string>
#include "stb_image.h"

#include <vector>
// Source courtesy of J. Manson
// http://josiahmanson.com/prose/optimize_ppm/


using namespace std;
void eat_comment(ifstream &f)
{
    char linebuf[1024];
    char ppp;
    while (ppp = f.peek(), ppp == '\n' || ppp == '\r')
        f.get();
    if (ppp == '#')
        f.getline(linebuf, 1023);
}


void load_ppm(ImageByte &img, const string &name)
{
    ifstream f(name.c_str(), ios::binary);
    if (f.fail())
    {
        cout << "Could not open file: " << name << endl;
        return;
    }

    // get type of file
    eat_comment(f);
    int mode = 0;
    string s;
    f >> s;
    if (s == "P5")
        mode = 5;
    else if (s == "P6")
        mode = 6;

    // get w
    eat_comment(f);
    f >> img.w;

    // get h
    eat_comment(f);
    f >> img.h;

    // get bits
    eat_comment(f);
    int bits = 0;
    f >> bits;

    // error checking
    if (mode != 5 && mode != 6)
    {
        cout << "Unsupported magic number" << endl;
        f.close();
        return;
    }
    if (img.w < 1)
    {
        cout << "Unsupported width: " << img.w << endl;
        f.close();
        return;
    }
    if (img.h < 1)
    {
        cout << "Unsupported height: " << img.h << endl;
        f.close();
        return;
    }
    if (bits < 1 || bits > 255)
    {
        cout << "Unsupported number of bits: " << bits << endl;
        f.close();
        return;
    }

    // load image data
    img.data.resize(img.w * img.h);

    if (mode == 6)
    {
        f.get();
        f.read( (char*)(img.data.data()) , 3 * img.w * img.h );
        
    }
    else if (mode == 5)
    {
        std::vector< char > res(img.data.size());
        f.get();
        f.read( (char*)(res.data()) , img.data.size() );
        for (unsigned int i = 0; i < img.data.size(); i++)
        {

            img.data[i] = Color(res[i]);
        }
    }

    // close file
    f.close();
}


void load_ppm( unsigned char * & pixels , unsigned int & w , unsigned int & h , const string &name , loadedFormat format)
{
    ifstream f(name.c_str(), ios::binary);
    if (f.fail())
    {
        cout << "Could not open file: " << name << endl;
        return;
    }

    // get type of file
    eat_comment(f);
    int mode = 0;
    string s;
    f >> s;
    if (s == "P5")
        mode = 5;
    else if (s == "P6")
        mode = 6;

    // get w
    eat_comment(f);
    f >> w;

    // get h
    eat_comment(f);
    f >> h;

    // get bits
    eat_comment(f);
    int bits = 0;
    f >> bits;

    // error checking
    if (mode != 5 && mode != 6)
    {
        cout << "Unsupported magic number" << endl;
        f.close();
        return;
    }
    if (w < 1)
    {
        cout << "Unsupported width: " << w << endl;
        f.close();
        return;
    }
    if (h < 1)
    {
        cout << "Unsupported height: " << h << endl;
        f.close();
        return;
    }
    if (bits < 1 || bits > 255)
    {
        cout << "Unsupported number of bits: " << bits << endl;
        f.close();
        return;
    }

    // load image data
    pixels = new unsigned char[3 * w * h];

    if (mode == 6)
    {
        f.get();
        f.read( (char*)&(pixels[0]) , 3 * w * h );
    }
    else if (mode == 5)
    {
        for (unsigned int i = 0; i < w * h; i++)
        {
            int v;
            f >> v;
            pixels[3*i + 0] = v;
            f >> v;
            pixels[3*i + 1] = v;
            f >> v;
            pixels[3*i + 2] = v;
        }
    }

    // close file
    f.close();

    if(format == rgb) {
        return;
    }

    if(format == rbg) {
        for (unsigned int i = 0; i < w * h; i++)
        {
            unsigned char r = pixels[3*i + 0] , g = pixels[3*i + 1] , b = pixels[3*i + 2];
            pixels[3*i + 0] = r;
            pixels[3*i + 1] = b;
            pixels[3*i + 2] = g;
        }
        return;
    }
}


ImageByte ImageByte::fromPPM(const string &name){
    ImageByte res;
    load_ppm(res, name);
    return res;
}

void ImageByte::saveHistogramData(string name, bool as_grey, bool cumulative) const{

    const char * nom_image = name.c_str();
    FILE *f_data;

    if( (f_data = fopen(nom_image, "wb")) == NULL){
        printf("\n[Export Histogramme] Pas d'acces en ecriture sur le fichier %s \n", nom_image);
        exit(EXIT_FAILURE);
    }

    if (cumulative){
        HistogramProbas histo = getHistogramProbaRepartition(as_grey);

        for (int i = 0; i <= 255; ++i){
            if (as_grey){
                fprintf(f_data,
                    "%d %f\n",
                    i,
                    histo[i][0]
                );
            }
            else {
                fprintf(f_data,
                    "%d %f %f %f\n",
                    i,
                    histo[i][0],
                    histo[i][1],
                    histo[i][2]
                    );
            }
        }
    }
    else{
        Histogram histo =getHistogramData(as_grey);

        for (int i = 0; i <= 255; ++i){
            if (as_grey){
                fprintf(f_data,
                    "%d %d\n",
                    i,
                    histo[i][0]
                );
            }
            else {
                fprintf(f_data,
                    "%d %d %d %d\n",
                    i,
                    histo[i][0],
                    histo[i][1],
                    histo[i][2]
                    );
            }
        }
    }
    fclose(f_data);
}

void ImageByte::saveHistogramProbabilisticData(string name, bool as_grey) const{

    const char * nom_image = name.c_str();
    FILE *f_data;

    if( (f_data = fopen(nom_image, "wb")) == NULL){
        printf("\n[Export Histogramme] Pas d'acces en ecriture sur le fichier %s \n", nom_image);
        exit(EXIT_FAILURE);
    }

    HistogramProbas histo = getHistogramProba(as_grey);
    HistogramProbas histoRep = getHistogramProbaRepartition(as_grey);

    for (int i = 0; i <= 255; ++i){
        if (as_grey){
            fprintf(f_data,
                "%d %f %f\n",
                i,
                histo[i][0],
                histoRep[i][0]
            );
        }
        else {
            fprintf(f_data,
                "%d %f %f %f %f\n",
                i,
                histo[i][0],
                histo[i][1],
                histo[i][2],
                (histoRep[i][0] + histoRep[i][1] + histoRep[i][2]) / 3.0
                );
        }
    }
    fclose(f_data);
}

void ImageByte::saveHistogramComparison(ImageByte other, string name, bool as_grey) const{

    const char * nom_image = name.c_str();
    FILE *f_data;

    if( (f_data = fopen(nom_image, "wb")) == NULL){
        printf("\n[Export Histogramme] Pas d'acces en ecriture sur le fichier %s \n", nom_image);
        exit(EXIT_FAILURE);
    }

    HistogramProbas histo = getHistogramProba(as_grey);
    HistogramProbas histo2 = other.getHistogramProba(as_grey);

    for (int i = 0; i <= 255; ++i){
        if (as_grey){
            fprintf(f_data,
                "%d %f %f\n",
                i,
                histo[i][0],
                histo2[i][0]
            );
        }
        else {
            std::cout << "dans saveHistogramComparison: RGB pas encore implémenté" << std::endl;
            exit(1);
        }
    }
    fclose(f_data);
}

Histogram ImageByte::getHistogramData(bool is_grey /*= false */) const{

    Histogram res({0, 0, 0});


    for (int u=0; u < w; ++u){
        for (int v=0; v < h; ++v){

            Color c = (*this)(u, v);
            if (is_grey){
                c = c.mean();
            }

            res[c.r][0] +=1;
            res[c.g][1] +=1;
            res[c.b][2] +=1;
        }
    }

    return res;
}

HistogramProbas ImageByte::getHistogramProba(bool as_grey) const{

    HistogramProbas res;

    Histogram histo = getHistogramData(as_grey);

    float s = w * h;
    for (int i = 0; i <= 255; ++i){
        res[i][0] = histo[i][0] / s;
        //std::cout << i << "  " << res[i][0]<< "  " << histo[i][0] << std::endl;
        res[i][1] = histo[i][1] / s;
        res[i][2] = histo[i][2] / s;
    }

    return res;
}

HistogramProbas ImageByte::getHistogramProbaRepartition(bool as_grey) const{

    HistogramProbas res;

    Histogram histo = getHistogramData(as_grey);

    double s = w * h;
    res[0][0] = histo[0][0] / s;
    res[0][1] = histo[0][1] / s;
    res[0][2] = histo[0][2] / s;

    for (int i = 1; i <= 255; ++i){
        res[i][0] = res[i-1][0] + histo[i][0] / s;
        res[i][1] = res[i-1][1] + histo[i][1] / s;
        res[i][2] = res[i-1][2] + histo[i][2] / s;
    }

    return res;
}

void saveAsPPM(string name, int w, int h, const std::vector<Color> & data){

    string ext = name.substr(name.find_last_of("."));
    bool as_pgm = false;

    if (ext == ".pgm") as_pgm = true;
    else if (! (ext == ".ppm")) name += ".ppm";
    
    const char * nom_image = name.c_str();

    FILE *f_image;

    if (as_pgm){

        int taille_image = w * h;

        std::vector< char > image_pgm(taille_image);
        for (int i = 0; i < data.size(); ++i){
            image_pgm[i] = (char) data[i].mean();
        }


        if( (f_image = fopen(nom_image, "wb")) == NULL){
            printf("\nPas d'acces en ecriture sur l'image %s \n", nom_image);
            exit(EXIT_FAILURE);
        }
        else{
            fprintf(f_image,"P5\r") ;                               /*ecriture entete*/
            fprintf(f_image,"%d %d\r255\r", w, h) ;

            if( (fwrite((char*)image_pgm.data(), sizeof(char), taille_image, f_image)) != (size_t) taille_image){
                printf("\nErreur de lecture de l'image %s \n", nom_image);
                exit(EXIT_FAILURE);
                }
            fclose(f_image);
        }
    }
    else {

        int taille_image = 3*w * h;

        if( (f_image = fopen(nom_image, "wb")) == NULL){
            printf("\nPas d'acces en ecriture sur l'image %s \n", nom_image);
            exit(EXIT_FAILURE);
        }
        else{
            fprintf(f_image,"P6\r") ;                               /*ecriture entete*/
            fprintf(f_image,"%d %d\r255\r", w, h) ;

            if( (fwrite((char*)data.data(), sizeof(char), taille_image, f_image))
                != (size_t)(taille_image))
            {
                printf("\nErreur d'ecriture de l'image %s \n", nom_image);
                exit(EXIT_FAILURE);
            }
            fclose(f_image);
        }
    }
}

void ImageByte::saveAs(string name, float quality) const{
    string ext = name.substr(name.find_last_of("."));

    char * data_start = (char * )data.data();

    if (ext == ".pgm" || ext == ".ppm") saveAsPPM(name, w, h, data);
    else if (ext == ".jpg" || ext == ".jpeg") stbi_write_jpg(name.c_str(), w, h, 3, data_start, 100 * quality);
    else {
        if (! (ext == ".png")) name += ".png";
        stbi_write_png(name.c_str(), w, h, 3, data_start, w * 3);
    }
}


ImageRGB ImageRGB::fromPPM(const string &name){
    return ImageRGB(
        ImageByte::fromPPM(name)
    );
}

ImageByte ImageRGB::toImage(bool has_neg) const{
        ImageByte res(w, h);
        for (int i = 0; i < w; ++i)
            for (int j=0; j< h; ++j)
                res(i, j) = Color((*this)(i, j) + Vec3(0.5) * (has_neg));
            
        return res;
}

Histogram ImageRGB::getHistogramData(bool as_grey) const{
    return toImage().getHistogramData(as_grey);
}

HistogramProbas ImageRGB::getHistogramProba(bool as_grey) const{
    return toImage().getHistogramProba(as_grey);
}

HistogramProbas ImageRGB::getHistogramProbaRepartition(bool as_grey) const{
    return toImage().getHistogramProbaRepartition(as_grey);
}

void ImageRGB::saveHistogramComparison(ImageByte other, string name, bool as_grey) const{
    return toImage().saveHistogramComparison( other,  name, as_grey);
}

void ImageRGB::saveHistogramData(string name, bool as_grey, bool cumulative) const{
    return toImage().saveHistogramData( name, as_grey, cumulative);
}

void ImageRGB::saveHistogramProbabilisticData(string name, bool as_grey) const{
    return toImage().saveHistogramProbabilisticData( name, as_grey);
}



Color ImageByte::getPixelSafe(int u, int v, RepeatMode repeat_mode) const{
    switch (repeat_mode){
        case RepeatMode::TILE:
            return (*this)(
                u % w,
                v % h
            );
            break;

        case RepeatMode::EXTEND:
            return (*this)(
                std::clamp(u, 0, w-1),
                std::clamp(v, 0, h-1)
            );
            break;
        
        case RepeatMode::MIRROR:

            u = u % (w * 2 );
            v = v % (h * 2 );

            if (u<0) u = -u;
            if (v<0) v = -v;

            if (u >= w) u = 2*w - u - 1;
            if (v >= h) v = 2*h - v - 1;

            return (*this)(
                u,
                v
            );
            break;
    }
    return (*this)(
                u,
                v
            );
}

ImageByte ImageByte::convolve(const Kernel & kernel, RepeatMode repeat_mode) const{
    ImageByte result(w, h);

    for (int i = 0; i < w; ++i){
        for (int j = 0; j < h; ++j){

            Vec3 val;

            //std::cout  << "nouvelle erosion" << std::endl;
            for (int u = -kernel.extent; u <= kernel.extent; ++u){
                for (int v = -kernel.extent; v <= kernel.extent; ++v){
                    
                    int k = i+u;
                    int l = j+v;

                    val += getPixelSafe(k, l).toVec3() * kernel(u, v);
                }
                //std::cout << "\n";
            }
            result(i, j) = Color(val);
        }
    }
    return result;
}

ImageByte ImageByte::mask_mix(const ImageByte & other, const ImageRGB & mask) const{
    ImageByte res(w, h);

    for (int i =0; i < w; ++i){
        for (int j =0; j < h; ++j){
            res(i, j) = (mask(i, j)[0] > 0.5)? (*this)(i, j): other(i, j);
        }
    }
    return res;
}


Vec3 ImageRGB::getPixelSafe(int u, int v, RepeatMode repeat_mode) const{

    switch (repeat_mode){
        case RepeatMode::TILE:
            return (*this)(
                u % w,
                v % h
            );
            break;

        case RepeatMode::EXTEND:
            return (*this)(
                std::clamp(u, 0, w-1),
                std::clamp(v, 0, h-1)
            );
            break;
        
        case RepeatMode::MIRROR:

            u = u % (w * 2 );
            v = v % (h * 2 );

            if (u<0) u = -u;
            if (v<0) v = -v;

            if (u >= w) u = 2*w - u - 1;
            if (v >= h) v = 2*h - v - 1;

            return (*this)(
                u,
                v
            );
            break;
    }
    return (*this)(
                u,
                v
            );
}

ImageRGB ImageRGB::convolve(const Kernel & kernel, RepeatMode repeat_mode) const{
    ImageRGB result(w, h);

    for (int i = 0; i < w; ++i){
        for (int j = 0; j < h; ++j){

            Vec3 val;

            for (int u = -kernel.extent; u <= kernel.extent; ++u){
                for (int v = -kernel.extent; v <= kernel.extent; ++v){
                    
                    int k = i+u;
                    int l = j+v;

                    val += getPixelSafe(k, l) * kernel(u, v);
                }
            }
            result(i, j) = val;
        }
    }
    return result;
}