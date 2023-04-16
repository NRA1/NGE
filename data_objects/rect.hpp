#ifndef NGE_RECT_HPP
#define NGE_RECT_HPP


class Rect
{
public:
    explicit Rect(float x = 0, float y = 0, float width = 0, float height = 0);

    float x() const;
    void setX(float x);
    float y() const;
    void setY(float y);
    float width() const;
    void setWidth(float width);
    float height() const;
    void setHeight(float height);

private:
    float x_;
    float y_;
    float width_;
    float height_;

    friend class SaveManager;
};


#endif //NGE_RECT_HPP
