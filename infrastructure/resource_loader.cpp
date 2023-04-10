#include "resource_loader.hpp"
#include "stb_image.hpp"

std::map<std::tuple<std::string, char, unsigned int>, Character> ResourceLoader::character_cache_;

std::optional<std::string> ResourceLoader::loadText(const std::string& file)
{
    std::filesystem::path path = fullPath(file);
    std::ifstream file_stream(path.string());
    if(!file_stream.is_open())
    {
        log() - Critical < "Failed to open file" < path.string();
        return std::nullopt;
    }

    std::string text;
    std::string line;
    while(getline(file_stream, line))
    {
        text.append(line);
        text.append("\n");
    }
    file_stream.close();
    if(file_stream.is_open())
    {
        log() - Critical < "Failed to close file" < path.string();
        return std::nullopt;
    }

    return text;
}

std::filesystem::path ResourceLoader::resourcesBasePath()
{
    boost::filesystem::path exec_path = boost::dll::program_location().parent_path() / RESOURCES_PREFIX_PATH / "data";
    return { exec_path.string() };
}

std::optional<Image> ResourceLoader::loadImageFromFile(const std::string &file)
{
    std::filesystem::path path = fullPath(file);
    int width, height, nr_components;
#ifdef __unix__
    const char *c_path = path.c_str();
#endif
#ifdef WIN32
    std::string str = path.string();
    const char *c_path = str.c_str();
#endif
    unsigned char *data = stbi_load(c_path, &width, &height, &nr_components, 0);
    if(!data)
    {
        log() - Critical < "Failed to load image from path" < path.string();
        stbi_image_free(data);
        return std::nullopt;
    }
    return Image(data, width, height, nr_components);
}

std::filesystem::path ResourceLoader::fullPath(const std::string &path)
{
    std::filesystem::path full_path;
    if(path.starts_with(":/"))
    {
        std::string actual_path = path.substr(2);
#if WIN32
        std::replace(actual_path.begin(), actual_path.end(), '/', '\\');
#endif
        full_path = resourcesBasePath() / actual_path;
    }
    else
    {
        std::string actual_path = path;
#if WIN32
        std::replace(actual_path.begin(), actual_path.end(), '/', '\\');
#endif
        full_path = std::filesystem::path(actual_path);
    }
    return full_path;
}

std::optional<Texture *>
ResourceLoader::loadTexture(const std::string &file, const std::string &directory, const std::string &type,
                            const aiScene *scene)
{
    std::optional<Image> image_opt;
    std::string path;
    if(const aiTexture *texture = scene->GetEmbeddedTexture(file.c_str()))
    {
        path = file;
        if(texture->mHeight != 0)
        {
            log() - LogLevel::Critical < "Loading of textures of uncompressed file formats is not supported.";
            return std::nullopt;
        }
        image_opt = loadImageFromMemory((unsigned char*)texture->pcData, texture->mWidth);
    }
    else
    {
        path = directory + '/' + file;
        image_opt = loadImageFromFile(path);
    }

    if(!image_opt.has_value()) return std::nullopt;
    Image image = image_opt.value();

    return textureFromImage(image, type, path);
}

std::optional<Image> ResourceLoader::loadImageFromMemory(const unsigned char *buffer, int len)
{
    int width, height, nr_components;
    unsigned char *data = stbi_load_from_memory(buffer, len, &width, &height, &nr_components, 0);
    if(!data)
    {
        log() - Critical < "Failed to load image from memory";
        stbi_image_free(data);
        return std::nullopt;
    }
    return Image(data, width, height, nr_components);
}

Texture *ResourceLoader::textureFromImage(Image &image, const std::string &type, const std::string &path)
{
    unsigned int texture_id;
    glGenTextures(1, &texture_id);

    GLenum format = GL_NONE;
    if(image.nrComponents() == 1) format = GL_RED;
    else if(image.nrComponents() == 3) format = GL_RGB;
    else if(image.nrComponents() == 4) format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, format, image.width(), image.height(), 0, format, GL_UNSIGNED_BYTE, image.data());
    image.destroy();
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return new Texture(texture_id, type, fullPath(path).string());
}

bool ResourceLoader::loadFont(const std::string &font, unsigned int size)
{
    if(!Game::initialized())
    {
        log() - Critical < "Game not initialized! Cannot load font";
        return false;
    }
    FT_Face face;
    if(FT_New_Face(Game::freeType(), fullPath(font).c_str(), 0, &face)) return false;
    FT_Set_Pixel_Sizes(face, 0, size);

    for(unsigned char c = 0; c < 128; c++)
    {
        if(FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            char ch = (char)c;
            log() - Critical < "FreeType failed to load glyph: " + std::string(&ch);
            continue;
        }

        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, (int)(face->glyph->bitmap.width), (int)(face->glyph->bitmap.rows), 0,
                     GL_RED,GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        character_cache_.insert(
                std::pair<std::tuple<std::string, char, unsigned int>, Character>(
                        std::tuple<std::string, char, unsigned int>(font, c, size),
                        Character(texture, Vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                                  Vec2(face->glyph->bitmap_left, face->glyph->bitmap_top), face->glyph->advance.x))
        );

    }

    FT_Done_Face(face);
    return true;
}

const Character &ResourceLoader::loadCharacter(const std::string &font, char character, unsigned int size)
{
    if(!character_cache_.contains(std::tuple<std::string, char, unsigned int>(font, character, size))
            && !loadFont(font, size))
    {
        log() - Critical < "Failed to load font:" < font < "Cannot load character. Falling to character fallback";
        return common_fallbacks::defaultCharacter;
    }
    if(!character_cache_.contains(std::tuple<std::string, char, unsigned int>(font, character, size)))
    {
        log() - Critical < "Character not in cache. Failing back to character fallback";
        return common_fallbacks::defaultCharacter;
    }
    return character_cache_[std::tuple<std::string, char, unsigned int>(font, character, size)];
}

void ResourceLoader::clearCaches()
{
    for(auto &character : character_cache_)
        character.second.destroy();
    character_cache_.clear();
}



