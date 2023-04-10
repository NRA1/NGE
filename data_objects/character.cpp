#include "character.hpp"
#include "glad/gl.h"

Character::Character(unsigned int texture_id, Vec2 size, Vec2 bearing, unsigned int advance)
        : texture_id_(texture_id), size_(size), bearing_(bearing), advance_(advance)
{

}

Character::Character() : texture_id_(0), size_(Vec2(0, 0)), bearing_(Vec2(0, 0)), advance_(0)
{

}

unsigned int Character::textureId() const
{
    return texture_id_;
}

void Character::setTextureId(unsigned int textureId)
{
    texture_id_ = textureId;
}

const Vec2 &Character::size() const
{
    return size_;
}

void Character::setSize(const Vec2 &size)
{
    size_ = size;
}

const Vec2 &Character::bearing() const
{
    return bearing_;
}

void Character::setBearing(const Vec2 &bearing)
{
    bearing_ = bearing;
}

unsigned int Character::advance() const
{
    return advance_;
}

void Character::setAdvance(unsigned int advance)
{
    advance_ = advance;
}

void Character::destroy()
{
    glDeleteTextures(1, &texture_id_);
}

