#include "bar_component.h"

#include <utility>
#include "../enums/component_type.hpp"

BarComponent::BarComponent(std::string name, unsigned int capacity, unsigned int value)
        : AbstractBarComponent(std::move(name)), capacity_(capacity), value_(value)
{

}

unsigned int BarComponent::capacity() const
{
    return capacity_;
}

unsigned int BarComponent::value() const
{
    return value_;
}

void BarComponent::adjustValue(int adjustment)
{
    value_ += adjustment;
}

void BarComponent::setValue(unsigned int value)
{
    value_ = value;
}

unsigned int BarComponent::type()
{
    return BarComponentType;
}

void BarComponent::objectChanged()
{

}
