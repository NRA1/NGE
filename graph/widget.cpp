#include "widget.hpp"
#include "../enums/layout_origin.hpp"

Widget::Widget(Widget *parent) : pos_(Vec2(0, 0)), z_pos_(0), visible_(true), layout_flags_(0),
                                 layout_origin_(LayoutOrigin::Center)
{
    if(parent != nullptr)
        parent->children_.push_back(this);
}

const Vec2 &Widget::pos() const
{
    return pos_;
}

void Widget::setPos(const Vec2 &pos)
{
    pos_ = pos;
}

void Widget::load()
{

}

void Widget::render(std::map<unsigned int, ShaderProgram *> &) const
{

}

void Widget::unload()
{

}

bool Widget::keyPressEvent(KeyPressEvent *)
{
    return false;
}

bool Widget::keyReleaseEvent(KeyReleaseEvent *)
{
    return false;
}

bool Widget::mouseMoveEvent(MouseMoveEvent *)
{
    return false;
}

void Widget::mouseEnterEvent()
{

}

void Widget::mouseLeaveEvent()
{

}

unsigned int Widget::requiredShaderPrograms() const
{
    return 0;
}

bool Widget::mousePressEvent(MousePressEvent *)
{
    return false;
}

bool Widget::mouseReleaseEvent(MouseReleaseEvent *)
{
    return false;
}

float Widget::zPos() const
{
    return z_pos_;
}

void Widget::setZPos(float z_pos)
{
    z_pos_ = z_pos;
}

unsigned int Widget::layoutFlags() const
{
    return layout_flags_;
}

void Widget::setLayoutFlags(unsigned int layout_flags)
{
    layout_flags_ = layout_flags;
}

unsigned int Widget::layoutOrigin() const
{
    return layout_origin_;
}

void Widget::setLayoutOrigin(unsigned int layout_origin)
{
    layout_origin_ = layout_origin;
}

bool Widget::visible() const
{
    return visible_;
}

void Widget::setVisible(bool visible)
{
    visible_ = visible;
}

const std::vector<Widget *> &Widget::children() const
{
    return children_;
}

Widget *Widget::parent() const
{
    return parent_;
}

Widget::~Widget()
{
    for(auto &child : children_)
        delete child;
}


