#ifndef NGE_ABSTRACT_BAR_COMPONENT_H
#define NGE_ABSTRACT_BAR_COMPONENT_H


#include "abstract_component.hpp"

class AbstractBarComponent : public AbstractComponent
{
public:
    AbstractBarComponent(std::string name);

    virtual unsigned int capacity() const = 0;
    virtual unsigned int value() const = 0;

    virtual void setValue(unsigned int value) = 0;
    virtual void adjustValue(int adjustment) = 0;

    unsigned int type() override;
};


#endif //NGE_ABSTRACT_BAR_COMPONENT_H
