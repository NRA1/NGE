#ifndef NGE_ABSTRACT_STAGE_HPP
#define NGE_ABSTRACT_STAGE_HPP

#include "../data_objects/size.hpp"
#include "../data_objects/events/event.hpp"

class AbstractStage
{
public:
    virtual void onAppearing() = 0;
    virtual void onDisappearing() = 0;

    virtual void viewportSizeChanged(Size size) = 0;
    virtual void handleEvent(Event *event) = 0;
    virtual void render(unsigned int time) = 0;

    virtual ~AbstractStage() = default;

};

#endif //NGE_ABSTRACT_STAGE_HPP
