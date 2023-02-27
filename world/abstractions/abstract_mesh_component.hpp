#ifndef NGE_ABSTRACT_MESH_COMPONENT_HPP
#define NGE_ABSTRACT_MESH_COMPONENT_HPP

#include "abstract_component.hpp"
#include "../../data_objects/box.hpp"
#include "../../infrastructure/logger.hpp"
#include "../../enums/component_type.hpp"
#include "../../data_objects/positioned_box.hpp"

class AbstractMeshComponent : public AbstractComponent
{
public:
    AbstractMeshComponent(std::string name) : AbstractComponent(name) { };

    unsigned int type() override;
    virtual void load() = 0;
    virtual void unload() = 0;
    virtual void render() = 0;
    virtual void setModelPosition() = 0;
    [[nodiscard]] virtual const PositionedBox &boundingBox() const = 0;

    ~AbstractMeshComponent() override = default;
};

#endif //NGE_ABSTRACT_MESH_COMPONENT_HPP
