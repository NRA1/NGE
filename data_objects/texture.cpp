#include "texture.hpp"

Texture::Texture(unsigned int id, std::string type, std::string path) : id_(id), type_(std::move(type)), path_(std::move(path))
{

}

unsigned int Texture::id() const
{
    return id_;
}

void Texture::setId(unsigned int id)
{
    id_ = id;
}

const std::string &Texture::type() const
{
    return type_;
}

void Texture::setType(const std::string &type)
{
    type_ = type;
}

const std::string &Texture::path() const
{
    return path_;
}

void Texture::setPath(const std::string &path)
{
    path_ = path;
}

Texture::~Texture()
{
    glDeleteTextures(1, &id_);
}
