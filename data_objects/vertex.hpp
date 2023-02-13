#ifndef NGE_VERTEX_HPP
#define NGE_VERTEX_HPP

#include "vec.hpp"

#define MAX_BONE_INFLUENCE 4

struct Vertex
{
    Vec3 Position;
    Vec3 Normal;
    Vec2 TexCoords;
    Vec3 Tangent;
    Vec3 Bitangent;

    int BoneIDs_[MAX_BONE_INFLUENCE];
    float Weights_[MAX_BONE_INFLUENCE];
};


#endif //NGE_VERTEX_HPP
