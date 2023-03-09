#pragma once
#ifndef NGE_X_11_WINDOW_HPP
#define NGE_X_11_WINDOW_HPP
#ifdef __unix__
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
    void poolEvents() override;
    void swapBuffers() override;
    void setCursorVisibility(bool visible) override;

    ~X11Window();

private:
    void windowStateChange(XConfigureEvent event);

    Display *display_;
    int screen_;
    Window window_;
    Atom wm_delete_window_;
    Cursor cursor_;

    GLXContext context_;
    Colormap colormap_;

    std::list<unsigned int> pressed_keys_;
    int pressed_modifiers_;
};

#include "x_11_window.inl"
#endif
#endif //NGE_X_11_WINDOW_HPP
