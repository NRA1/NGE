#ifndef NGE_DEBUG_BOX_HPP
#define NGE_DEBUG_BOX_HPP


#include "object.hpp"
#include <glm/matrix.hpp>

class DebugBox : public Object
{
public:
    DebugBox(std::string name, const PositionedBox &box);

    void load() override;
    void render() override;
    void unload() override;

    void setBox(const PositionedBox &box);

private:
    void loadBox();

    PositionedBox box_;
    ShaderProgram *shader_program_;
    unsigned int VBO, VAO, EBO;
};


#endif //NGE_DEBUG_BOX_HPP
