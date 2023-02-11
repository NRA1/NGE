#ifndef NGE_BOX_HPP
#define NGE_BOX_HPP


class Box
{
public:
    Box() = default;
    Box(int width, int height, int depth);

    int width() const;
    void setWidth(int width);
    int height() const;
    void setHeight(int height);
    int depth() const;
    void setDepth(int depth);

private:
    int width_;
    int height_;
    int depth_;
};


#endif //NGE_BOX_HPP
