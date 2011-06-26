#ifndef PPB_PPM_H
#define PPB_PPM_H

#include <fstream>

#define PPM_MAGIC "P3"

typedef struct _rgb_t
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
} rgb_t;

typedef struct _image_t
{
    int width;
    int height;
    rgb_t *buf;
} image_t;

void write_ppm(image_t* img, const char* fname)
{
    int i, j;
    int w = img->width;
    int h = img->height;

    std::ofstream ofs(fname);

    ofs << PPM_MAGIC << std::endl
        << w << " " << h << std::endl
        << "255" << std::endl;
    
    for (int i=0; i<h; ++i)
    {
        for (int j=0; j<w; ++j)
        {
            ofs << static_cast<int>(img->buf[i*w+j].r) << " "
                << static_cast<int>(img->buf[i*w+j].g) << " "
                << static_cast<int>(img->buf[i*w+j].b) << std::endl;
        }
    }
}

void new_image(image_t* img, int w, int h)
{
    img->width = w;
    img->height = h;
    img->buf = new rgb_t[w*h];
}

void delete_image(image_t *img)
{
    delete img->buf;
}

#endif /* PPB_PPM_H */
