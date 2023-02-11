#include "image.hpp"
#include "stb_image.hpp"

Image::Image(unsigned char *data, int width, int height, int nr_components)
        : data_(data), width_(width), height_(height), nr_components_(nr_components)
{

}

unsigned char *Image::data() const
{
    return data_;
}

int Image::width() const
{
    return width_;
}

int Image::height() const
{
    return height_;
}

int Image::nrComponents() const
{
    return nr_components_;
}

void Image::destroy()
{
    stbi_image_free(data_);
}