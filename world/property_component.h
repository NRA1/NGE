#ifndef NGE_PROPERTY_COMPONENT_H
#define NGE_PROPERTY_COMPONENT_H


#include "abstractions/abstract_component.hpp"
#include "../enums/component_type.hpp"
#include "../infrastructure/save_manager.hpp"

template<typename T>
class PropertyComponent : public AbstractComponent
{
public:
    PropertyComponent(std::string name, T value) : AbstractComponent(name)
    {
        value_ = value;
    }

    explicit PropertyComponent(std::ifstream &ifs) : AbstractComponent(ifs)
    {
        ifs.read((char*)&value_, sizeof(value_));
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

    void dump(std::ofstream &ofs) override
    {
        if(!ofs.is_open()) throw "File not open";
        ofs.write((char*)&value_, sizeof(value_));
    }

private:
    T value_;
};


#endif //NGE_PROPERTY_COMPONENT_H
