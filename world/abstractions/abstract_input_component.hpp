#ifndef NGE_ABSTRACT_INPUT_COMPONENT_HPP
#define NGE_ABSTRACT_INPUT_COMPONENT_HPP


#include "abstract_component.hpp"
#include "../../data_objects/events/key_press_event.hpp"
#include "../../data_objects/events/key_release_event.hpp"
#include "../../enums/component_type.hpp"
#include "../../data_objects/events/mouse_move_event.hpp"

class AbstractInputComponent : public AbstractComponent
{
public:
    AbstractInputComponent(const std::string &name);
    AbstractInputComponent(std::ifstream &ifs);

    unsigned int type() override;
    virtual bool keyPressEvent(KeyPressEvent *event) = 0;
    virtual bool keyReleaseEvent(KeyReleaseEvent *event) = 0;
    virtual bool mouseMoveEvent(MouseMoveEvent *event) = 0;
};


#endif //NGE_ABSTRACT_INPUT_COMPONENT_HPP
