#ifndef NGE_RESOURCE_LOADER_HPP
#define NGE_RESOURCE_LOADER_HPP

#include <filesystem>
#include <optional>
#include <fstream>
#include <boost/dll/runtime_symbol_info.hpp>
#include "logger.hpp"
#include "../data_objects/image.hpp"
#include "../data_objects/texture.hpp"
#include "assimp/scene.h"

class ResourceLoader
{
public:
    static std::optional<std::string> loadText(const std::string& file);
    static std::optional<Image> loadImageFromFile(const std::string &file);
    static std::optional<Image> loadImageFromMemory(const unsigned char *buffer, int len);
    static std::filesystem::path fullPath(const std::string &path);
    static std::optional<Texture*> loadTexture(const std::string &file, const std::string &directory, const std::string &type, const aiScene *scene);
    static Texture *textureFromImage(Image &image, const std::string &type, const std::string &path);

private:
    static std::filesystem::path resourcesBasePath();
};


#endif //NGE_RESOURCE_LOADER_HPP
