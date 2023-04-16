#include "abstract_bar_component.h"
#include "../../enums/component_type.hpp"

#include <utility>

AbstractBarComponent::AbstractBarComponent(std::string name)
        : AbstractComponent(std::move(name))
{

}

unsigned int AbstractBarComponent::type()
{
    return (int)AbstractBarComponentType;
}

AbstractBarComponent::AbstractBarComponent(std::ifstream &ifs) : AbstractComponent(ifs)
{

}
