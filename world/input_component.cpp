#include "input_component.hpp"

InputComponent::InputComponent(const std::string &name) : AbstractInputComponent(name), manipulation_obj_(nullptr),
                                                          last_mouse_move_pos(std::nullopt)
{
}

bool InputComponent::keyPressEvent(KeyPressEvent *event)
{
    if(!event->isRepeated())
    {
        Vec3 vec;
        if(manipulation_obj_->velocity().has_value()) vec = manipulation_obj_->velocity().value();
        else vec = Vec3(0, 0, 0);

        if(event->key() == Key::KeyW)
            vec.z = vec.z + 10;
        else if(event->key() == Key::KeyA)
            vec.x = vec.x + 10;
        else if(event->key() == Key::KeyD)
            vec.x = vec.x - 10;
        else if(event->key() == Key::KeyS)
            vec.z = vec.z - 10;

        manipulation_obj_->setVelocity(vec);
    }
    return true;
}

bool InputComponent::keyReleaseEvent(KeyReleaseEvent *event)
{
    Vec3 vec;
    if(manipulation_obj_->velocity().has_value()) vec = manipulation_obj_->velocity().value();
    else vec = Vec3(0, 0, 0);

    if(event->key() == Key::KeyW)
        vec.z = vec.z - 10;
    else if(event->key() == Key::KeyA)
        vec.x = vec.x - 10;
    else if(event->key() == Key::KeyD)
        vec.x = vec.x + 10;
    else if(event->key() == Key::KeyS)
        vec.z = vec.z + 10;
    manipulation_obj_->setVelocity(vec);
    return true;
}

void InputComponent::objectChanged()
{
    manipulation_obj_ = object()->motionVector().generateManipulationObject(true);
}

bool InputComponent::mouseMoveEvent(MouseMoveEvent *event)
{
    Rotation rot = object()->rotation();
    rot.setRoll(rot.roll() + ((float)event->x() / 100));
    object()->setRotation(rot);
    return true;
}

InputComponent::~InputComponent()
{
    object()->motionVector().destroyManipulationObject(manipulation_obj_);
}

InputComponent::InputComponent(std::ifstream &ifs) : AbstractInputComponent(ifs), manipulation_obj_(nullptr),
                                                     last_mouse_move_pos(std::nullopt)
{
}

unsigned int InputComponent::type()
{
    return ComponentType::InputComponentType;
}
