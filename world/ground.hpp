#ifndef NGE_GROUND_HPP
#define NGE_GROUND_HPP


#include <string>
#include "../data_objects/rect.hpp"
#include "../data_objects/texture.hpp"
#include "../infrastructure/resource_loader.hpp"
#include "../infrastructure/common_fallbacks.hpp"
#include "../data_objects/shader_program.hpp"
#include "../data_objects/sprite.hpp"

class Ground
{
public:
    Ground(Rect bounds, const std::string &diffuse = "", const std::string &specular = "", const std::string &normal = "",
           const std::string &height = "");

    void render() const;

    const Rect &bounds() const;
    void setShaderProgram(ShaderProgram **shader_program);

private:
    void load(const std::string &diffuse, const std::string &specular, const std::string &normal,
              const std::string &height);

    Rect bounds_;
    std::optional<Sprite> sprite_;

    ShaderProgram **shader_program_;

    friend class SaveManager;
};


#endif //NGE_GROUND_HPP
