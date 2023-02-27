#include "abstract_input_component.hpp"

AbstractInputComponent::AbstractInputComponent(const std::string &name) : AbstractComponent(name)
{

}

unsigned int AbstractInputComponent::type()
{
    return AbstractInputComponentType;
}

