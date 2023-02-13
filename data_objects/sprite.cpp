#include "sprite.hpp"

#include <utility>

Sprite::Sprite(Rect bounding_rect, std::string texture) : texture_(nullptr), bounding_rect_(bounding_rect)
{
    path_ = std::move(texture);
}

void Sprite::render(ShaderProgram *shader_program) const
{
    shader_program->use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_->id());

    shader_program->setInt("texture_diffuse1", 0);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Sprite::load()
{
    float vertices[] = {
            // positions                                                                                          // texture coords
            bounding_rect_.x() + bounding_rect_.width(),  bounding_rect_.y() + bounding_rect_.height(), 0.0f,     1.0f, 1.0f, // top right
            bounding_rect_.x() + bounding_rect_.width(), bounding_rect_.y(), 0.0f,                                1.0f, 0.0f, // bottom right
            bounding_rect_.x(), bounding_rect_.y(), 0.0f,                                                         0.0f, 0.0f, // bottom left
            bounding_rect_.x(),  bounding_rect_.y() + bounding_rect_.width(), 0.0f,                               0.0f, 1.0f  // top left
    };
    unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
    };
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    std::optional<Image> image = ResourceLoader::loadImageFromFile(path_);

    if (image.has_value())
    {
        texture_ = ResourceLoader::textureFromImage(image.value(), "texture_diffuse", path_);
    }
    else
    {
        log() - Critical < "Failed to load sprite texture";
        log() - Critical < "Failing back to default texture";
        texture_ = common_fallbacks::defaultTexture("texture_diffuse");
    }
}

void Sprite::unload()
{
    delete texture_;
    texture_ = nullptr;
}

