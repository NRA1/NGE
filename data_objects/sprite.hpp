#ifndef NGE_SPRITE_HPP
#define NGE_SPRITE_HPP


#include "shader_program.hpp"
#include "rect.hpp"

class Sprite
{
public:
    Sprite(Rect bounding_rect, std::string texture);

    void load();
    void render(ShaderProgram *shader_program) const;
    void unload();

private:
    unsigned int VBO, VAO, EBO;

    Texture *texture_;
    std::string path_;
    Rect bounding_rect_;
};


#endif //NGE_SPRITE_HPP
