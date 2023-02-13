#ifndef NGE_SIZE_HPP
#define NGE_SIZE_HPP


class Size
{
public:
    Size(float width, float height);
    float width() const;
    void setWidth(float width);
    float height() const;
    void setHeight(float height);

    bool isValid() const;

private:
    float width_;
    float height_;
};


#endif //NGE_SIZE_HPP
