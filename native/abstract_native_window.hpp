#ifndef NGE_ABSTRACT_NATIVE_WINDOW_HPP
#define NGE_ABSTRACT_NATIVE_WINDOW_HPP

#include "../data_objects/events/event.hpp"
#include "../data_objects/size.hpp"

template<class T>
class AbstractNativeWindow
{
public:
    AbstractNativeWindow(const std::string &title, Size size);


    virtual void show() = 0;
    virtual void hide() = 0;
    virtual bool shouldClose();
    virtual void poolEvents() = 0;
    virtual void swapBuffers() = 0;
    virtual void setSubscriber(T *subscriber);
    virtual void setViewportChangeHandler(void (T::*handler)(Size));
    virtual const Size &size() const;
    virtual void setEventHandler(void (T::*handler)(Event*));
    virtual void setCursorVisibility(bool visible);
    virtual bool cursorVisible() const;

protected:
    std::string title_;
    Size size_;
    bool should_close_;
    bool cursor_visible_ = true;

    T *subscriber_;
    void (T::*viewport_change_handler_)(Size);
    void (T::*event_handler_)(Event *);
};

#include "abstract_native_window.inl"

#endif //NGE_ABSTRACT_NATIVE_WINDOW_HPP
