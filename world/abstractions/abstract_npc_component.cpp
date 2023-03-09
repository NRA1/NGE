#include "abstract_npc_component.hpp"
#include "../../enums/component_type.hpp"

#include <utility>

AbstractNPCComponent::AbstractNPCComponent(std::string name) : AbstractComponent(std::move(name))
{

}

unsigned int AbstractNPCComponent::type()
{
    return AbstractNPCComponentType;
}
