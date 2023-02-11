#include "common_fallbacks.hpp"

Texture *common_fallbacks::defaultTexture(const std::string &type)
{
    unsigned int texture_id;
    glGenTextures(1, &texture_id);
    return new Texture(texture_id, type, "default");
}