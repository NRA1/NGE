#ifndef NGE_INPUT_COMPONENT_HPP
#define NGE_INPUT_COMPONENT_HPP

#include "abstractions/abstract_input_component.hpp"
#include "../data_objects/motion_manipulation_object.hpp"
#include "object.hpp"
#include "../enums/key.hpp"

class InputComponent : public AbstractInputComponent
{
public:
    InputComponent(const std::string &name);

    bool keyPressEvent(KeyPressEvent *event) override;
    bool keyReleaseEvent(KeyReleaseEvent *event) override;
    bool mouseMoveEvent(MouseMoveEvent *event) override;

    void objectChanged() override;

    ~InputComponent() override;

private:
    MotionManipulationObject *manipulation_obj_;
    std::optional<Vec2> last_mouse_move_pos;
};


#endif //NGE_INPUT_COMPONENT_HPP
