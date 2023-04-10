#ifndef NGE_RECT_WIDGET_HPP
#define NGE_RECT_WIDGET_HPP

#include <glad/gl.h>
#include "widget.hpp"
#include "../data_objects/shader_program.hpp"

class RectWidget : public Widget
{
public:
    RectWidget(Rect rect, Vec4 color);

    void load() override;
    void render(std::map<unsigned int, ShaderProgram *> &shader_program) const override;
    void unload() override;

    bool mouseMoveEvent(MouseMoveEvent *event) override;
    void mouseEnterEvent() override;
    void mouseLeaveEvent() override;

    Rect boundingRect() const override;
    unsigned int requiredShaderPrograms() const override;

private:
    bool highlighted_;

    Rect rect_;
    Vec4 color_;
    unsigned int VBO_, VAO_, EBO_;
};


#endif //NGE_RECT_WIDGET_HPP