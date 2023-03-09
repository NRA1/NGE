#ifndef NGE_SHADER_PROGRAM_HPP
#define NGE_SHADER_PROGRAM_HPP

#include <string>
#include <glad/gl.h>
#include <optional>
#include "../infrastructure/logger.hpp"
#include "../infrastructure/resource_loader.hpp"
#include "../infrastructure/common_fallbacks.hpp"
#include "../infrastructure/game.hpp"
#include "vec.hpp"

class ShaderProgram
{
public:
    ShaderProgram(const char *vertex_path, const char *fragment_path);

    void use() const;
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec2(const std::string &name, Vec2 value) const;
    void setMat4(const std::string &name, Mat4 &value) const;

    ~ShaderProgram();

private:
    unsigned int id_;
};


#endif //NGE_SHADER_PROGRAM_HPP
