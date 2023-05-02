#pragma once
#ifndef NGE_WIN_32_WINDOW_HPP
#define NGE_WIN_32_WINDOW_HPP
#ifdef WIN32
#include <string>
#include <windows.h>
#include <Windowsx.h>
#include <glad/wgl.h>
#include "abstract_native_window.hpp"
#include "../data_objects/size.hpp"
#include "../enums/key.hpp"
#include "../enums/modifier.hpp"
#include "../data_objects/events/key_press_event.hpp"
#include "../data_objects/events/key_release_event.hpp"
#include "../data_objects/events/mouse_move_event.hpp"
#include "../data_objects/events/mouse_press_event.h"
#include "../data_objects/events/mouse_release_event.h"
#include "native_helpers.hpp"

template<class T>
class Win32Window : public AbstractNativeWindow<T>
{
public:
    Win32Window(const std::string &title, Size size);

    void show() override;
    void hide() override;
    void poolEvents() override;
    void swapBuffers() override;
    void setCursorVisibility(bool visible) override;

    ~Win32Window();

private:
    static LRESULT CALLBACK windowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

    void clipCursor();
    void resetCursor();
    RECT windowRect();


    HWND window_;
    HDC device_context_;
    HGLRC opengl_context_;

    int pressed_modifiers_;
    bool shown_;
};

#include "win_32_window.inl"

#endif
#endif //NGE_WIN_32_WINDOW_HPP
