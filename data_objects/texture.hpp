#ifndef NGE_TEXTURE_HPP
#define NGE_TEXTURE_HPP

#include <string>
#include <utility>
#include <glad/gl.h>
#include <optional>

class Texture
{
public:
    Texture(unsigned int id, std::string type, std::string path);

    [[nodiscard]] unsigned int id() const;
    void setId(unsigned int id);
    [[nodiscard]] const std::string &type() const;
    void setType(const std::string &type);
    [[nodiscard]] const std::string &path() const;
    void setPath(const std::string &path);

private:
    unsigned int id_;
    std::string type_;
    std::string path_;
};


#endif //NGE_TEXTURE_HPP
