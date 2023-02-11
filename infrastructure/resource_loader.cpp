#include "resource_loader.hpp"
#include "stb_image.hpp"

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
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nr_components, 0);
    if(!data)
    {
        log() - Critical < "Failed to load image from path" < path;
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
        full_path = resourcesBasePath() / path.substr(2);
    }
    else full_path = std::filesystem::path(path);
    return full_path;
}

std::optional<Texture *>
ResourceLoader::loadTexture(const std::string &file, const std::string &directory, const std::string &type,
                            const aiScene *scene)
{
    std::string texture_path = directory + '/' + file;
    std::optional<Image> image_opt;
    if(const aiTexture *texture = scene->GetEmbeddedTexture(file.c_str()))
    {
        if(texture->mHeight != 0)
        {
            log() - LogLevel::Critical < "Loading of textures of uncompressed file formats is not supported.";
            return std::nullopt;
        }
        image_opt = loadImageFromMemory((unsigned char*)texture->pcData, texture->mWidth);
    }
    else
    {
        image_opt = loadImageFromFile(texture_path);
    }

    if(!image_opt.has_value()) return std::nullopt;
    Image image = image_opt.value();

    unsigned int texture_id;
    glGenTextures(1, &texture_id);

    GLenum format;
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

    return new Texture(texture_id, type, fullPath(file));
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

