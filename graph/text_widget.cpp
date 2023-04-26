#include "text_widget.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "../enums/shader_program_type.hpp"

#include <utility>

TextWidget::TextWidget(std::string text, std::string font, unsigned int font_size, Vec4 color, Widget *parent)
        : Widget(parent), text_(std::move(text)), font_(std::move(font)), font_size_(font_size), color_(color),
          bounding_rect_(Rect())
{

}

TextWidget::TextWidget(std::string text, Widget *parent)
        : Widget(parent), text_(std::move(text)), font_(":/fonts/arial.ttf"), font_size_(20), color_(1, 1, 1, 1)
{

}

void TextWidget::load()
{
    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);
    glBindVertexArray(VAO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

    for(char c : text_)
        characters_[c] = ResourceLoader::loadCharacter(font_, c, font_size_);

    calculateBoundingRect();
}

void TextWidget::render(std::map<unsigned int, ShaderProgram *> &shader_programs) const
{
    shader_programs[ShaderProgramType::TextType]->use();
    shader_programs[ShaderProgramType::TextType]->setVec4("textColor", color_);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO_);

    float x = 0, y = 0;

    std::string::const_iterator c;
    for(c = text_.begin(); c != text_.end(); c++)
    {
        Character ch = (*characters_.find(*c)).second;

        float xpos = x + ch.bearing().x;
        float ypos = y - (ch.size().y - ch.bearing().y);

        float w = ch.size().x;
        float h = ch.size().y;

        float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }
        };

        glBindTexture(GL_TEXTURE_2D, ch.textureId());
        glBindBuffer(GL_ARRAY_BUFFER, VBO_);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (float)(ch.advance() >> 6);
    }
}

void TextWidget::unload()
{
    characters_.clear();
    glDeleteVertexArrays(1, &VAO_);
    glDeleteBuffers(1, &VBO_);
}

Rect TextWidget::boundingRect() const
{
    return bounding_rect_;
}

unsigned int TextWidget::requiredShaderPrograms() const
{
    return ShaderProgramType::TextType;
}

void TextWidget::calculateBoundingRect()
{

}

const std::string &TextWidget::text() const
{
    return text_;
}

void TextWidget::setText(const std::string &text)
{
    characters_.clear();
    text_ = text;
    for(char c : text_)
        characters_[c] = ResourceLoader::loadCharacter(font_, c, font_size_);

    calculateBoundingRect();
}

const std::string &TextWidget::font() const
{
    return font_;
}

void TextWidget::setFont(const std::string &font)
{
    unload();
    font_ = font;
    load();
}

unsigned int TextWidget::fontSize() const
{
    return font_size_;
}

void TextWidget::setFontSize(unsigned int fontSize)
{
    unload();
    font_size_ = fontSize;
    load();
}

const Vec4 &TextWidget::color() const
{
    return color_;
}

void TextWidget::setColor(const Vec4 &color)
{
    unload();
    color_ = color;
    load();
}
