#ifndef NGE_RECT_HPP
#define NGE_RECT_HPP


class Rect
{
public:
    explicit Rect(int x = 0, int y = 0, int width = 0, int height = 0);

    int x() const;
    void setX(int x);
    int y() const;
    void setY(int y);
    int width() const;
    void setWidth(int width);
    int height() const;
    void setHeight(int height);

private:
    int x_;
    int y_;
    int width_;
    int height_;
};


#endif //NGE_RECT_HPP
