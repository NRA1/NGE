#include "object.hpp"
#include "mesh_component.hpp"

Object::Object() : features_(0)
{

}

void Object::addComponent(AbstractComponent *component)
{
    component->setObject(this);
    components_.push_back(component);
    if(component->type() == MeshComponentType)
        mesh_components_.push_back((AbstractMeshComponent*)component);
    features_ |= component->type();
}

void Object::removeComponent(const std::string &name)
{
    AbstractComponent *component = removeComponentFromVector<AbstractComponent>(components_, name);
    if(component->type() == MeshComponentType) removeComponentFromVector<AbstractMeshComponent>(mesh_components_, name);

    delete component;
}

int Object::features() const
{
    return features_;
}

void Object::load()
{
    for (AbstractMeshComponent *component : mesh_components_)
        component->load();
}

void Object::unload()
{
    for (AbstractMeshComponent *component : mesh_components_)
        component->unload();
}

void Object::render()
{
    for (AbstractMeshComponent *component : mesh_components_)
        component->render();

}

const Vec3 &Object::position() const
{
    return position_;
}

void Object::setPosition(const Vec3 &position)
{
    position_ = position;
}

const Vec3 &Object::rotation() const
{
    return rotation_;
}

void Object::setRotation(const Vec3 &rotation)
{
    rotation_ = rotation;
}

WorldStage *Object::stage() const
{
    return stage_;
}

void Object::setStage(WorldStage *stage)
{
    stage_ = stage;
}

Object::~Object()
{
    for (AbstractComponent *component : components_)
    {
        delete component;
    }
}

