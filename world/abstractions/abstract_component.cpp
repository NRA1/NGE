#include "abstract_component.hpp"
#include "../../infrastructure/save_manager.hpp"

#include <utility>

AbstractComponent::AbstractComponent(std::string name) : name_(std::move(name)), object_(nullptr)
{

}

const std::string &AbstractComponent::name() const
{
    return name_;
}

void AbstractComponent::setName(std::string name)
{
    name_ = std::move(name);
}

Object *AbstractComponent::object() const
{
    return object_;
}

void AbstractComponent::setObject(Object *object)
{
    object_ = object;
    objectChanged();
}

void AbstractComponent::dump(std::ofstream &ofs)
{
    if(!ofs.is_open()) throw "File not open";
    char name[20];
    for(unsigned int i = 0; i < name_.size(); i++)
        name[i] = name_[i];
    name[name_.size()] = '\0';
    ofs.write(name, sizeof(name));
}

AbstractComponent::AbstractComponent(std::ifstream &ifs) : object_(nullptr)
{
    if(!ifs.is_open()) throw "File not open";
    char name[20];
    ifs.read(name, sizeof(name));
    name_ = std::string(name);
}

