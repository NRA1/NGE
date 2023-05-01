#include "input_component.hpp"

InputComponent::InputComponent(const std::string &name) : AbstractInputComponent(name), manipulation_obj_(nullptr),
                                                          last_mouse_move_pos(std::nullopt), direction_(Vec2(0, 0))
{
}

bool InputComponent::keyPressEvent(KeyPressEvent *event)
{
    if(!event->isRepeated())
    {
        if(event->key() == Key::KeyW)
        {
            direction_.y += 1;
        }
        else if(event->key() == Key::KeyA)
        {
            direction_.x += 1;
        }
        else if(event->key() == Key::KeyD)
        {
            direction_.x -= 1;
        }
        else if(event->key() == Key::KeyS)
        {
            direction_.y -= 1;
        }

        manipulation_obj_->setVelocity(glm::normalize(Vec3(direction_.x, 0, direction_.y)) * 10.0f);
    }
    return true;
}

bool InputComponent::keyReleaseEvent(KeyReleaseEvent *event)
{
    if(event->key() == Key::KeyW)
    {
        direction_.y -= 1;
    }
    else if(event->key() == Key::KeyA)
    {
        direction_.x -= 1;
    }
    else if(event->key() == Key::KeyD)
    {
        direction_.x += 1;
    }
    else if(event->key() == Key::KeyS)
    {
        direction_.y += 1;
    }

    Vec3 vec = Vec3(direction_.x, 0, direction_.y);
    manipulation_obj_->setVelocity(glm::length(vec) != 0 ? glm::normalize(vec) * 10.0f : vec);
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
