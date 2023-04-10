#pragma once
#ifndef NGE_COMMON_FALLBACKS_HPP
#define NGE_COMMON_FALLBACKS_HPP

#include <string>
#include "../data_objects/texture.hpp"
#include "../data_objects/character.hpp"

namespace common_fallbacks
{
    const std::string vertex_shader_source_fallback = "#version 330 core\n"
                                                      "\n"
                                                      "layout (location = 0) in vec3 aPos;\n"
                                                      "\n"
                                                      "void main()\n"
                                                      "{\n"
                                                      "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                                      "}";

    const std::string fragment_shader_source_fallback = "#version 330 core\n"
                                                        "out vec4 FragColor;\n"
                                                        "\n"
                                                        "void main()\n"
                                                        "{\n"
                                                        "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                                        "} ";

    Texture *defaultTexture(const std::string &type);
    const Character defaultCharacter = Character(0, Vec2(0, 0), Vec2(0, 0), 0);
}

#endif //NGE_COMMON_FALLBACKS_HPP
