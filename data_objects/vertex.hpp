#ifndef NGE_VERTEX_HPP
#define NGE_VERTEX_HPP

#include "vec.hpp"

#define MAX_BONE_INFLUENCE 4

struct Vertex
{
//public:
//    Vertex();
//    Vertex(Vec3 position, Vec3 normal, Vec2 tex_coords);
//
//    [[nodiscard]] const Vec3 &position() const;
//    void setPosition(const Vec3 &position);
//    [[nodiscard]] const Vec3 &normal() const;
//    void setNormal(const Vec3 &normal);
//    [[nodiscard]] const Vec2 &texCoords() const;
//    void setTexCoords(const Vec2 &texCoords);
//
//    friend class Mesh;
//private:
    Vec3 Position;
    Vec3 Normal;
    Vec2 TexCoords;
    Vec3 Tangent;
    Vec3 Bitangent;

    int BoneIDs_[MAX_BONE_INFLUENCE];
    float Weights_[MAX_BONE_INFLUENCE];
};


#endif //NGE_VERTEX_HPP
