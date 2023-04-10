#ifndef NGE_BAR_COMPONENT_H
#define NGE_BAR_COMPONENT_H


#include "abstractions/abstract_bar_component.h"

class BarComponent : public AbstractBarComponent
{
public:
    BarComponent(std::string name, unsigned int capacity, unsigned int value);
    unsigned int capacity() const override;
    unsigned int value() const override;
    void adjustValue(int adjustment) override;
    void setValue(unsigned int value) override;

    unsigned int type() override;
    void objectChanged() override;

private:
    unsigned int capacity_;
    unsigned int value_;
};


#endif //NGE_BAR_COMPONENT_H
