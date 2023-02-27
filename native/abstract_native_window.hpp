#ifndef NGE_ABSTRACT_NATIVE_WINDOW_HPP
#define NGE_ABSTRACT_NATIVE_WINDOW_HPP

#include "../data_objects/events/event.hpp"
#include "../data_objects/size.hpp"

template<class T>
class AbstractNativeWindow
{
public:
    virtual void show() = 0;
    virtual void hide() = 0;
    virtual bool shouldClose() = 0;
    virtual void poolEvents() = 0;
    virtual void swapBuffers() = 0;
    virtual void setSubscriber(T *subscriber) = 0;
    virtual void setViewportChangeHandler(void (T::*handler)(Size)) = 0;
    virtual const Size &size() const = 0;
    virtual void setEventHandler(void (T::*handler)(Event*)) = 0;
    virtual void setCursorVisibility(bool visible);
    virtual bool cursorVisible() const;

private:
    bool cursor_visible_ = true;
};

#include "abstract_native_window.inl"

#endif //NGE_ABSTRACT_NATIVE_WINDOW_HPP
