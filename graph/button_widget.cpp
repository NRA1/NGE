#include "button_widget.hpp"
#include "glm/ext/matrix_transform.hpp"

#include <utility>

ButtonWidget::ButtonWidget(std::string text, Rect rect, bool (*handler)(), Vec4 background_color, Vec4 font_color,
                           unsigned int font_size, std::string font)
        : rect_widget_(rect, background_color), text_widget_(std::move(text), std::move(font), font_size, font_color),
          background_color_(background_color), handler_(handler)
{
    rect_widget_.setZPos(zPos());
    text_widget_.setPos(Vec2(rect.x() + 10, rect.y() + 10));
    text_widget_.setZPos(zPos() + 0.1f);
}

Rect ButtonWidget::boundingRect() const
{
    return rect_widget_.boundingRect();
}

unsigned int ButtonWidget::requiredShaderPrograms() const
{
    return text_widget_.requiredShaderPrograms() | rect_widget_.requiredShaderPrograms();
}

void ButtonWidget::load()
{
    rect_widget_.load();
    text_widget_.load();
}

void ButtonWidget::render(std::map<unsigned int, ShaderProgram *> &shader_programs) const
{
    Widget::render(shader_programs);
    Mat4 mat = Mat4(1.0f);
    mat = glm::translate(mat, Vec3(rect_widget_.pos(), rect_widget_.zPos()));
    for(auto &sp : shader_programs)
        sp.second->setMat4("widget", mat);
    rect_widget_.render(shader_programs);
    mat = Mat4(1.0f);
    mat = glm::translate(mat, Vec3(text_widget_.pos(), text_widget_.zPos()));
    for(auto &sp : shader_programs)
        sp.second->setMat4("widget", mat);
    text_widget_.render(shader_programs);
}

void ButtonWidget::unload()
{
    text_widget_.unload();
    rect_widget_.unload();
}

void ButtonWidget::setHandler(bool (*handler)())
{
    handler_ = handler;
}

bool ButtonWidget::mousePressEvent(MousePressEvent *)
{
    if(handler_ != nullptr)
        return handler_();
    return false;
}

void ButtonWidget::mouseEnterEvent()
{
    rect_widget_.setColor(Vec4(background_color_.r * 0.8f, background_color_.g * 0.8f, background_color_.b * 0.8f,
                               background_color_.a));
}

void ButtonWidget::mouseLeaveEvent()
{
    rect_widget_.setColor(background_color_);
}
