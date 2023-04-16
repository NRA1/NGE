#ifndef NGE_TEXT_WIDGET_HPP
#define NGE_TEXT_WIDGET_HPP


#include "widget.hpp"

class TextWidget : public Widget
{
public:
    TextWidget(std::string text, std::string font = ":/fonts/arial.ttf", unsigned int font_size = 20, Vec4 color = Vec4(1, 1, 1, 1));

    virtual void load() override;
    virtual void render(std::map<unsigned int, ShaderProgram *> &shader_programs) const override;
    virtual void unload() override;

    Rect boundingRect() const override;
    unsigned int requiredShaderPrograms() const override;

    const std::string &text() const;
    void setText(const std::string &text);
    const std::string &font() const;
    void setFont(const std::string &font);
    unsigned int fontSize() const;
    void setFontSize(unsigned int fontSize);
    const Vec4 &color() const;
    void setColor(const Vec4 &color);

private:
    void calculateBoundingRect();

    std::string text_;
    std::string font_;
    unsigned int font_size_;
    Vec4 color_;
    std::map<char, Character> characters_;
    unsigned int VAO_, VBO_;

    Rect bounding_rect_;
};


#endif //NGE_TEXT_WIDGET_HPP
