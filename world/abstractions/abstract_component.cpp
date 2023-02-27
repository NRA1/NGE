#include "abstract_component.hpp"

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