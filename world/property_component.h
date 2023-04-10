#ifndef NGE_PROPERTY_COMPONENT_H
#define NGE_PROPERTY_COMPONENT_H


#include "abstractions/abstract_component.hpp"
#include "../enums/component_type.hpp"

template<typename T>
class PropertyComponent : public AbstractComponent
{
public:
    PropertyComponent(std::string name, T value) : AbstractComponent(name)
    {
        value_ = value;
    }

    T value() const
    {
        return value_;
    }

    void setValue(T value)
    {
        value_ = value;
    }

    void objectChanged() override { }
    unsigned int type() override
    {
        return PropertyComponentType;
    }

private:
    T value_;
};


#endif //NGE_PROPERTY_COMPONENT_H
