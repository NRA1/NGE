#include "object.hpp"

Object::Object() : capabilities_(0)
{

}

void Object::addComponent(AbstractComponent *component)
{
    component->setObject(this);
    components_.push_back(component);
    if(component->capabilities() & Capability::MeshRenderer) mesh_components_.push_back(component);
    capabilities_ |= component->capabilities();
}

void Object::removeComponent(const std::string &name)
{
    AbstractComponent *component = removeComponentFromVector(components_, name);
    if(component->capabilities() & Capability::MeshRenderer) removeComponentFromVector(mesh_components_, name);
    delete component;
}

int Object::capabilities() const
{
    return capabilities_;
}

AbstractComponent *Object::removeComponentFromVector(std::vector<AbstractComponent *> &vector, const std::string& name)
{
    for (unsigned long i = 0; i < vector.size(); ++i)
    {
        if(vector[i]->name() == name)
        {
            AbstractComponent *component = vector[i];
            vector.erase(vector.begin() + i);
            return component;
        }
    }
    return nullptr;
}

void Object::load()
{
    for (AbstractComponent *component : mesh_components_)
        component->load();
}

void Object::unload()
{
    for (AbstractComponent *component : mesh_components_)
        component->unload();
}

void Object::render()
{
    for (AbstractComponent *component : mesh_components_)
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

