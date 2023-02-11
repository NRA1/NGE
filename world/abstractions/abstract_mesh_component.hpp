#ifndef NGE_ABSTRACT_MESH_COMPONENT_HPP
#define NGE_ABSTRACT_MESH_COMPONENT_HPP

#include "abstract_component.hpp"
#include "../../data_objects/box.hpp"
#include "../../infrastructure/logger.hpp"

class AbstractMeshComponent : public AbstractComponent
{
public:
    AbstractMeshComponent(std::string name) : AbstractComponent(name) { };

    virtual void load() { log() - Info < "Called base"; };
    virtual void unload() = 0;
    virtual void render() { log() - Info < "Called base"; };
    virtual void setModelPosition() {};
    [[nodiscard]] virtual const Box &boundingBox() const { return *new Box();};

    virtual ~AbstractMeshComponent() = default;
};

#endif //NGE_ABSTRACT_MESH_COMPONENT_HPP
