#include "rect_widget.hpp"
#include "../enums/shader_program_type.hpp"

RectWidget::RectWidget(Rect rect, Vec4 color) : rect_(rect), color_(color), VBO_(0), VAO_(0), EBO_(0)
{

}

void RectWidget::load()
{
    float vertices[] = {
            rect_.x() + rect_.width(),  rect_.y() + rect_.height(), 0.0f,  // top right
            rect_.x() + rect_.width(),  rect_.y(), 0.0f,  // bottom right
            rect_.x(), rect_.y(), 0.0f,  // bottom left
            rect_.x(),  rect_.y() + rect_.height(), 0.0f   // top left
    };
    unsigned int indices[] = {  // note that we start from 0!
            0, 1, 3,  // first Triangle
            1, 2, 3   // second Triangle
    };

    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);
    glGenBuffers(1, &EBO_);

    glBindVertexArray(VAO_);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void RectWidget::render(std::map<unsigned int, ShaderProgram *> &shader_programs) const
{
    shader_programs[ShaderProgramType::ShapeType]->use();
    shader_programs[ShaderProgramType::ShapeType]->setVec4("color", color_);
    glBindVertexArray(VAO_);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void RectWidget::unload()
{
    glDeleteVertexArrays(1, &VAO_);
    glDeleteBuffers(1, &VBO_);
    glDeleteBuffers(1, &EBO_);

    VAO_ = 0;
    VBO_ = 0;
    EBO_ = 0;
}

Rect RectWidget::boundingRect() const
{
    return rect_;
}

unsigned int RectWidget::requiredShaderPrograms() const
{
    return ShaderProgramType::ShapeType;
}

const Rect &RectWidget::rect() const
{
    return rect_;
}

void RectWidget::setRect(const Rect &rect)
{
    rect_ = rect;
}

const Vec4 &RectWidget::color() const
{
    return color_;
}

void RectWidget::setColor(const Vec4 &color)
{
    color_ = color;
}
