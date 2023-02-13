#ifndef NGE_COMPONENT_TYPE_HPP
#define NGE_COMPONENT_TYPE_HPP

enum ComponentType
{
    // ComponentType: first six bits signify type (used to identify inside the game engine, the next six bytes are left
    // for game implementation to identify its component implementations
    MeshComponentType = 0x000001000000,
    MeshModelComponentType = 0x000001000001
};

#endif //NGE_COMPONENT_TYPE_HPP
