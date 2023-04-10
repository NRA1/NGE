#include "widget.hpp"

Widget::Widget() : pos_(Vec2(0, 0))
{

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

