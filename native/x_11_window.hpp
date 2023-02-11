#pragma once
#ifndef NGE_X_11_WINDOW_HPP
#define NGE_X_11_WINDOW_HPP

#include <string>
#include <list>
#include <algorithm>
#include "../infrastructure/logger.hpp"
#include <glad/gl.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/XKBlib.h>
#include <glad/glx.h>

#include "abstract_native_window.hpp"
#include "../data_objects/size.hpp"
#include "../data_objects/events/key_press_event.hpp"
#include "native_helpers.hpp"
#include "../enums/modifier.hpp"

template<class T>
class X11Window : public AbstractNativeWindow<T>
{
public:
    X11Window(const std::string& title, Size size);

    void show() override;
    void hide() override;
    bool shouldClose() override;
    void poolEvents() override;
    void swapBuffers() override;
    void setSubscriber(T *subscriber) override;
    void setViewportChangeHandler(void (T::*handler)(Size)) override;
    void setEventHandler(void (T::*handler)(Event*)) override;
    const Size &size() const override;


    ~X11Window();

private:
    void windowStateChange(XConfigureEvent event);

    std::string title_;
    Size size_;

    Display *display_;
    int screen_;
    Window window_;
    Atom wm_delete_window_;

    GLXContext context_;
    Colormap colormap_;

    bool should_close_;

    T *subscriber_;
    void (T::*viewport_change_handler_)(Size);
    void (T::*event_handler_)(Event *);

    std::list<unsigned int> pressed_keys_;
    int pressed_modifiers_;
};

#include "x_11_window.inl"
#endif //NGE_X_11_WINDOW_HPP
