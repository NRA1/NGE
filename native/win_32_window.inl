#include "win_32_window.hpp"
template<class T>
Win32Window<T>::Win32Window(const std::string &title, Size size) : AbstractNativeWindow<T>(title, size), pressed_modifiers_(0), shown_(false)
{
    TCHAR *window_title = new TCHAR[title.size() + 1];
    window_title[title.size()] = 0;
    std::copy(title.begin(), title.end(), window_title);

    HINSTANCE hinstance = Game::winmainConfig().Instance;

    WNDCLASSEX wcex = { };
    wcex.cbSize = sizeof (WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wcex.lpfnWndProc = &Win32Window<T>::windowProc;
    wcex.hInstance = hinstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    wcex.lpszClassName = window_title;
    wcex.cbWndExtra = sizeof(LONG_PTR);

    ATOM wndclass = RegisterClassEx(&wcex);
    if(wndclass == NULL)
    {
        log() - Fatal < "Failed to create window class. Faulting";
        Game::fault();
    }

    window_ = CreateWindow(window_title, window_title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                             (HWND) NULL, (HMENU) NULL, hinstance, (LPVOID)NULL);

    if(!window_)
    {
        log() - Fatal < "Failed to create Win32 window. Faulting";
        Game::fault();
    }

    SetWindowLongPtr(window_, 0, (LONG_PTR)this);

    device_context_ = GetDC(window_);
    if(device_context_ == nullptr)
    {
        DestroyWindow(window_);
        log() - Fatal < "Failed to get window device context. Faulting";
        Game::fault();
    }

    PIXELFORMATDESCRIPTOR pfd = { };
    pfd.nSize = sizeof(pfd);
    pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_TYPE_RGBA;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 32;
    pfd.iLayerType = PFD_MAIN_PLANE;
    pfd.cStencilBits = 8;

    int format = ChoosePixelFormat(device_context_, &pfd);
    if(format == 0 || SetPixelFormat(device_context_, format, &pfd) == FALSE)
    {
        ReleaseDC(window_, device_context_);
        DestroyWindow(window_);
        log() - Fatal < "Failed to set a compatible pixel format. Faulting";
        Game::fault();
    }

    HGLRC temp_context = nullptr;
    if((temp_context = wglCreateContext(device_context_)) == nullptr)
    {
        ReleaseDC(window_, device_context_);
        DestroyWindow(window_);
        log() - Fatal < "Failed to create the initial rendering context. Faulting";
        Game::fault();
    }

    wglMakeCurrent(device_context_, temp_context);
    gladLoaderLoadWGL(device_context_);

    int attributes[] = {
            WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
            WGL_CONTEXT_MINOR_VERSION_ARB, 2,
            WGL_CONTEXT_FLAGS_ARB,
            WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
            0
    };

    if((opengl_context_ = wglCreateContextAttribsARB(device_context_, nullptr, attributes)) == nullptr)
    {
        wglDeleteContext(temp_context);
        ReleaseDC(window_, device_context_);
        DestroyWindow(window_);
        log() - Fatal < "Failed to create the final rendering context. Faulting";
        Game::fault();
    }
    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(temp_context);
    wglMakeCurrent(device_context_, opengl_context_);

    if(!gladLoaderLoadGL())
    {
        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(opengl_context_);
        ReleaseDC(window_, device_context_);
        DestroyWindow(window_);
        log() - Fatal < "Failed to load Glad. Faulting";
        Game::fault();
    }
}

template<class T>
void Win32Window<T>::show()
{
    ShowWindow(window_, SW_NORMAL);
    UpdateWindow(window_);
    shown_ = true;
    clipCursor();
}

template<class T>
void Win32Window<T>::hide()
{
    ShowWindow(window_, SW_HIDE);
    UpdateWindow(window_);
    shown_ = false;
    clipCursor();
}

template<class T>
void Win32Window<T>::poolEvents()
{
    MSG msg = { };

    while(PeekMessage(&msg, window_, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

template<typename T>
LRESULT CALLBACK Win32Window<T>::windowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    if(message == WM_CREATE || message == WM_GETMINMAXINFO || message == WM_NCCALCSIZE || message == WM_NCCREATE) return DefWindowProc(hwnd, message, wparam, lparam);

    LONG_PTR win_lptr = GetWindowLongPtr(hwnd, 0);
    if (!win_lptr)
    {
        log() - Critical < "Failed to get window user pointer. Cannot handle the message. Skipping" < (int)message;
        return DefWindowProc(hwnd, message, wparam, lparam);
    }

    Win32Window<T>* win = (Win32Window<T>*)win_lptr;

    Event* dispatch_event = nullptr;

    switch (message)
    {
    case WM_QUIT:
    case WM_DESTROY:
    case WM_CLOSE:
        win->should_close_ = true;
        break;
    case WM_SIZE:
    {
        int width = LOWORD(lparam);
        int height = HIWORD(lparam);
        if (height != win->size_.height() || width != win->size_.width())
        {
            win->size_ = Size((float)width, (float)height);
            if (win->subscriber_ != nullptr && win->viewport_change_handler_ != nullptr)
                (win->subscriber_->*win->viewport_change_handler_)(win->size_);
        }
        break;
    }
    case WM_KEYDOWN:
    {
        Key key = native::nativeToKey(wparam);
        if (key == KeyEscape)
        {
            win->should_close_ = true;
            return 0;
        }
        int modifiers = win->pressed_modifiers_;
        int repeated = lparam & 0x40000000;
        Modifier modifier = modifier::modifierFromKey(key);
        if (modifier != NoModifier && !(win->pressed_modifiers_ & NoModifier))
            win->pressed_modifiers_ |= modifier;
        dispatch_event = new KeyPressEvent(key, modifiers, repeated);
        break;
    }
    case WM_KEYUP:
    {
        Key key = native::nativeToKey(wparam);
        int modifiers = win->pressed_modifiers_;
        dispatch_event = new KeyReleaseEvent(key, modifiers);
        break;
    }
    case WM_MOUSEMOVE:
    {
        dispatch_event = new MouseMoveEvent((int)(GET_X_LPARAM(lparam) - win->size().width() / 2), (int)(GET_Y_LPARAM(lparam) - win->size().width() / 2), win->pressed_modifiers_);
        win->resetCursor();
        break;
    }
    default:
        return DefWindowProc(hwnd, message, wparam, lparam);
    }

    if (dispatch_event != nullptr)
    {
        if (win->subscriber_ == nullptr) log() - Debug < "No subscriber set, ignoring event";
        else if (win->event_handler_ == nullptr) log() - Debug < "Not event handler set, ignoring event";
        else (win->subscriber_->*win->event_handler_)(dispatch_event);

        delete dispatch_event;
        dispatch_event = nullptr;
    }

    return 0;
}

template<class T>
void Win32Window<T>::swapBuffers()
{
    SwapBuffers(device_context_);
}

template<class T>
void Win32Window<T>::setCursorVisibility(bool visible)
{
    ShowCursor(visible);
    clipCursor();
    AbstractNativeWindow<T>::setCursorVisibility(visible);
}

template<typename T>
void Win32Window<T>::clipCursor()
{
    if (this->cursor_visible_ || !shown_)
    {
        if (!ClipCursor(nullptr))
            log() - Critical < "Failed to clip cursor";
    }
    else
    {
        RECT rect = windowRect();
        if (!ClipCursor(&rect))
            log() - Critical < "Failed to clip cursor";
    }
}

template<class T>
inline void Win32Window<T>::resetCursor()
{
    if (this->cursorVisible()) return;

    RECT rect = windowRect();
    if (!SetCursorPos(rect.left + (rect.right - rect.left) / 2, rect.top + (rect.bottom - rect.top) / 2))
        log() - Critical < "Failed to reset cursor position";
}

template<class T>
inline RECT Win32Window<T>::windowRect()
{
    RECT rect;
    if (!GetClientRect(window_, &rect))
    {
        log() - Critical < "Failed to get window rect";
        return RECT();
    }
    POINT p;
    p.x = rect.left;
    p.y = rect.top;
    int success = 0;
    success |= ClientToScreen(window_, &p);
    rect.left = p.x;
    rect.top = p.y;
    p.x = rect.right;
    p.y = rect.bottom;
    success |= ClientToScreen(window_, &p);
    if (!success)
    {
        log() - Critical < "Failed to convert window rect to screen coordiantes";
        return RECT();
    }
    rect.right = p.x;
    rect.bottom = p.y;
    return rect;
}

template<class T>
Win32Window<T>::~Win32Window()
{
    wglDeleteContext(opengl_context_);
    ReleaseDC(window_, device_context_);
    DestroyWindow(window_);
}