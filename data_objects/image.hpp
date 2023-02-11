#ifndef NGE_IMAGE_HPP
#define NGE_IMAGE_HPP

class Image
{
public:
    Image(unsigned char *data, int width, int height, int nr_components);
    void destroy();

    unsigned char *data() const;
    int width() const;
    int height() const;
    int nrComponents() const;


private:
    unsigned char *data_;
    int width_, height_, nr_components_;
};

#endif //NGE_IMAGE_HPP
