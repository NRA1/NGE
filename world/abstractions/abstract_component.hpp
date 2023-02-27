#ifndef NGE_ABSTRACT_COMPONENT_HPP
#define NGE_ABSTRACT_COMPONENT_HPP

#include <string>
#include "../../data_objects/box.hpp"
#include "../../data_objects/shader_program.hpp"

class Object;
class WorldStage;

class AbstractComponent
{
public:
    explicit AbstractComponent(std::string name);
    void setName(std::string name);
    [[nodiscard]] const std::string &name() const;
    Object *object() const;
    void setObject(Object *object);
//    virtual int features() = 0;
    virtual void objectChanged() = 0;
    virtual unsigned int type() = 0;

//    virtual void load() = 0;
//    virtual void unload() = 0;
//    virtual void render() = 0;
//    virtual void setModelPosition() = 0;
//    [[nodiscard]] virtual const Box &boundingBox() const = 0;

    virtual ~AbstractComponent() = default;

private:
    std::string name_;
    Object *object_;
};

#endif //NGE_ABSTRACT_COMPONENT_HPP
