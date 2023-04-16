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

void BarComponent::dump(std::ofstream &ofs)
{
    if(!ofs.is_open()) throw "File not open";
    AbstractComponent::dump(ofs);
    ofs.write((char*)&capacity_, sizeof(capacity_));
    ofs.write((char*)&value_, sizeof(value_));
}

BarComponent::BarComponent(std::ifstream &ifs) : AbstractBarComponent(ifs)
{
    ifs.read((char*)&capacity_, sizeof(capacity_));
    ifs.read((char*)&value_, sizeof(value_));
}
