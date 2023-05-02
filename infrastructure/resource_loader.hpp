#ifndef NGE_RESOURCE_LOADER_HPP
#define NGE_RESOURCE_LOADER_HPP

#include <filesystem>
#include <optional>
#include <fstream>
#include <string>
#include <boost/dll/runtime_symbol_info.hpp>
#include <map>
#include "logger.hpp"
#include "../data_objects/image.hpp"
#include "../data_objects/texture.hpp"
#include "../data_objects/character.hpp"
#include "assimp/scene.h"
#include "game.hpp"
#include "common_fallbacks.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

class ResourceLoader
{
public:
    static std::optional<std::string> loadText(const std::string& file);
    static std::optional<Image> loadImageFromFile(const std::string &file);
    static std::optional<Image> loadImageFromMemory(const unsigned char *buffer, int len);
    static std::filesystem::path fullPath(const std::string &path);
    static std::optional<Texture*> loadTexture(const std::string &file, const std::string &directory, const std::string &type, const aiScene *scene);
    static Texture *textureFromImage(Image &image, const std::string &type, const std::string &path);
    static const Character &loadCharacter(const std::string &font, char character, unsigned int size);

    static void clearCaches();

private:
    static std::filesystem::path resourcesBasePath();
    static bool loadFont(const std::string &font, unsigned int size);

    static std::map<std::tuple<std::string, char, unsigned int>, Character> character_cache_;
};


#endif //NGE_RESOURCE_LOADER_HPP
