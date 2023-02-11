#ifndef NGE_SIZE_HPP
#define NGE_SIZE_HPP


class Size
{
public:
    Size(int width, int height);
    int width() const;
    void setWidth(int width);
    int height() const;
    void setHeight(int height);

    bool isValid() const;

private:
    int width_;
    int height_;
};


#endif //NGE_SIZE_HPP
