#ifndef NGE_CHARACTER_HPP
#define NGE_CHARACTER_HPP


#include "vec.hpp"

class Character
{
public:
    Character();
    Character(unsigned int texture_id, Vec2 size, Vec2 bearing, unsigned int advance);

    unsigned int textureId() const;
    void setTextureId(unsigned int textureId);
    const Vec2 &size() const;
    void setSize(const Vec2 &size);
    const Vec2 &bearing() const;
    void setBearing(const Vec2 &bearing);
    unsigned int advance() const;
    void setAdvance(unsigned int advance);

    void destroy();

private:
    unsigned int texture_id_;
    Vec2 size_;
    Vec2 bearing_;
    unsigned int advance_;
};


#endif //NGE_CHARACTER_HPP
