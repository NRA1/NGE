#ifndef NGE_ABSTRACT_MESH_COMPONENT_HPP
#define NGE_ABSTRACT_MESH_COMPONENT_HPP

#include "abstract_component.hpp"
#include "../../data_objects/box.hpp"
#include "../../infrastructure/logger.hpp"
#include "../../enums/component_type.hpp"

class AbstractMeshComponent : public AbstractComponent
{
public:
    AbstractMeshComponent(std::string name) : AbstractComponent(name) { };

    int type() override;
    virtual void load() = 0;
    virtual void unload() = 0;
    virtual void render() = 0;
    virtual void setModelPosition() = 0;
    [[nodiscard]] virtual const Box &boundingBox() const = 0;

    ~AbstractMeshComponent() override = default;
};

#endif //NGE_ABSTRACT_MESH_COMPONENT_HPP
