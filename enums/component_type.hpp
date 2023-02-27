#ifndef NGE_COMPONENT_TYPE_HPP
#define NGE_COMPONENT_TYPE_HPP

enum ComponentType
{
    // ComponentType: first 8 bits signify type (used to identify inside the game engine, the next 16 bits are left
    // for game implementation to identify its component implementations
    AbstractMeshComponentType = 0b0000000100000000,
    MeshComponentType = 0b0000000100000001,
    AbstractInputComponentType = 0b0000001000000000,
    InputComponentType = 0b0000001000000001
};

#endif //NGE_COMPONENT_TYPE_HPP
