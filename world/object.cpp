#include "object.hpp"

#include <utility>
#include "mesh_component.hpp"

Object::Object(std::string name) : features_(0), user_types_(0), name_(std::move(name)), position_(Vec3(0, 0, 0)),
                                   rotation_(0, 0, 0)
{

}

void Object::addComponent(AbstractComponent *component)
{
    component->setObject(this);
    components_.push_back(component);
    if(component->type() & AbstractMeshComponentType)
        mesh_components_.push_back((AbstractMeshComponent*)component);
    if(component->type() & AbstractInputComponentType)
        input_components_.push_back((AbstractInputComponent*)component);
    features_ |= component->type();
}

void Object::removeComponent(const std::string &name)
{
    AbstractComponent *component = removeComponentFromVector<AbstractComponent>(components_, name);
    if(component->type() & AbstractMeshComponentType) removeComponentFromVector<AbstractMeshComponent>(mesh_components_, name);
    if(component->type() & AbstractInputComponentType) removeComponentFromVector<AbstractInputComponent>(input_components_, name);

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

void Object::keyPressEvent(KeyPressEvent *event)
{
    for(const auto &component : input_components_)
        if(component->keyPressEvent(event)) break;
}

void Object::keyReleaseEvent(KeyReleaseEvent *event)
{
    for(const auto &component : input_components_)
        if(component->keyReleaseEvent(event)) break;
}

void Object::mouseMoveEvent(MouseMoveEvent *event)
{
    for(const auto &component : input_components_)
        if(component->mouseMoveEvent(event)) break;
}

const Vec3 &Object::position() const
{
    return position_;
}

void Object::setPosition(const Vec3 &position)
{
    position_ = position;
}

const Rotation &Object::rotation() const
{
    return rotation_;
}

void Object::setRotation(const Rotation &rotation)
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

MotionVector &Object::motionVector()
{
    return motion_vector_;
}

PositionedBox Object::boundingBox() const
{
    if(mesh_components_.empty()) return {};
    PositionedBox bounds = mesh_components_[0]->boundingBox();
    for(unsigned int i = 1; i < mesh_components_.size(); i++)
        bounds.unite(mesh_components_[i]->boundingBox());
    return bounds;
}

Object::~Object()
{
    for (AbstractComponent *component : components_)
    {
        delete component;
    }
}

int Object::collisionDamage() const
{
    return collision_damage_;
}

void Object::setCollisionDamage(int collision_damage)
{
    collision_damage_ = collision_damage;
}

int Object::userTypes() const
{
    return user_types_;
}

void Object::setUserTypes(int types)
{
    user_types_ = types;
}

const std::string &Object::name() const
{
    return name_;
}


