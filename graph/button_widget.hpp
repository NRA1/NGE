#ifndef NGE_BUTTON_WIDGET_HPP
#define NGE_BUTTON_WIDGET_HPP


#include "widget.hpp"
#include "rect_widget.hpp"
#include "text_widget.hpp"

class ButtonWidget : public Widget
{
public:
    ButtonWidget(std::string text, Rect rect, bool (*handler)(), Vec4 background_color, Vec4 font_color,
                 unsigned int font_size = 20, std::string font = ":/fonts/arial.ttf");

    Rect boundingRect() const override;
    unsigned int requiredShaderPrograms() const override;
    void load() override;
    void render(std::map<unsigned int, ShaderProgram *> &shader_programs) const override;
    void unload() override;

    bool mousePressEvent(MousePressEvent *event) override;
    void mouseEnterEvent() override;
    void mouseLeaveEvent() override;

    void setHandler(bool (*handler)());

private:
    RectWidget rect_widget_;
    TextWidget text_widget_;

    Vec4 background_color_;

    bool (*handler_)();
};


#endif //NGE_BUTTON_WIDGET_HPP
