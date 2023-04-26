#include "../interface/game_window.hpp"
#include "../data_objects/events/key_release_event.hpp"
#include "../data_objects/events/mouse_move_event.hpp"
#include "../data_objects/events/mouse_press_event.h"
#include "../data_objects/events/mouse_release_event.h"

template <class T>
X11Window<T>::X11Window(const std::string& title, Size size)
        : AbstractNativeWindow<T>(title, size),
          cursor_(None), pressed_modifiers_(Modifier::NoModifier)
{
    display_ = XOpenDisplay((char*)nullptr);
    if(display_ == nullptr)
    {
        log() - Fatal < "Cannot connect to X server";
        Game::fault();
    }

    wm_delete_window_ = XInternAtom(display_, "WM_DELETE_WINDOW", False);
    screen_ = DefaultScreen(display_);
    Window root = RootWindow(display_, screen_);
    Visual *visual = DefaultVisual(display_, screen_);

    colormap_ = XCreateColormap(display_, root, visual, AllocNone);

    XSetWindowAttributes attributes;
    attributes.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | StructureNotifyMask;
    attributes.colormap = colormap_;

    window_ = XCreateWindow(display_, root, 0, 0, this->size().width(), this->size().height(), 0, DefaultDepth(display_, screen_),
                                 InputOutput, visual, CWColormap | CWEventMask, &attributes);
    XSetWMProtocols(display_, window_, &wm_delete_window_, 1);
    XStoreName(display_, window_, title.c_str());
    Bool detectable_supported = false;
    Bool enabled = XkbSetDetectableAutoRepeat(display_, True, &detectable_supported);
    if(!detectable_supported) log() - Critical < "Detectable auto repeat not supported, keyboard input might not work correctly!";
    else if(!enabled) log() - Critical < "Could not enable detectable auto repeat, keyboard input might not work correctly!";
    XSync(display_, false);

    if(!window_)
    {
        log() - Fatal < "Unable to create window";
        Game::fault();
    }

    int glx_version = gladLoaderLoadGLX(display_, screen_);
    if(!glx_version)
    {
        log() - Fatal < "Unable to load GLX";
        Game::fault();
    }

    log() - Info < "Loaded GLX" < GLAD_VERSION_MAJOR(glx_version) <= "." <= GLAD_VERSION_MINOR(glx_version);

    GLint visual_attributes[] = { GLX_RENDER_TYPE, GLX_RGBA_BIT, GLX_DOUBLEBUFFER, 1, None };
    int num_fbc = 0;
    GLXFBConfig *fbc = glXChooseFBConfig(display_, screen_, visual_attributes, &num_fbc);

    GLint context_attributes[] = {
            GLX_CONTEXT_MAJOR_VERSION_ARB, 3, GLX_CONTEXT_MINOR_VERSION_ARB, 3, GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB, None
    };

    context_ = glXCreateContextAttribsARB(display_, fbc[0], NULL, 1, context_attributes);
    if(!context_)
    {
        log() - Fatal < "Failed to create context";
        Game::fault();
    }

    glXMakeCurrent(display_, window_, context_);

    int gl_version = gladLoaderLoadGL();
    if(!gl_version)
    {
        log() - Fatal < "Unable to load GL";
    }

    log() - Info < "Loaded GL" < GLAD_VERSION_MAJOR(gl_version) <= "." <= GLAD_VERSION_MINOR(glx_version);

    XWindowAttributes gwa;
    XGetWindowAttributes(display_, window_, &gwa);
    this->size_ = Size(gwa.width, gwa.height);
}

template <class T>
void X11Window<T>::show()
{
    XMapRaised(display_, window_);
    XSync(display_, false);
}

template <class T>
void X11Window<T>::hide()
{
    XUnmapWindow(display_, window_);
    XSync(display_, false);
}

template <class T>
void X11Window<T>::poolEvents()
{
    XEvent event;
    while (XCheckTypedWindowEvent(display_, window_, ClientMessage, &event))
    {
        if(((long unsigned int)event.xclient.data.l[0]) == wm_delete_window_)
        {
            this->should_close_ = true;
            return;
        }
    }

    Event *dispatch_event = nullptr;
    std::vector<unsigned int> skip;
    while (XCheckWindowEvent(display_, window_, ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | StructureNotifyMask, &event))
    {
        if(!skip.empty() && skip[0] == 0) continue;

        if(event.type == ConfigureNotify)
            windowStateChange(event.xconfigure);
        else if(event.type == KeyPress)
        {
            KeySym sym = XLookupKeysym(&event.xkey, 0);
            Key key = native::nativeToKey(sym);
            int modifiers = pressed_modifiers_;
            bool repeated = true;
            if(std::find(pressed_keys_.begin(), pressed_keys_.end(), event.xkey.keycode) == pressed_keys_.end())
            {
                repeated = false;
                pressed_keys_.push_front(event.xkey.keycode);
                Modifier modifier = modifier::modifierFromKey(key);
                if(modifier != NoModifier) pressed_modifiers_ |= modifier;
            }
            dispatch_event = new KeyPressEvent(key, modifiers, repeated);
        }
        else if(event.type == KeyRelease)
        {
            pressed_keys_.remove(event.xkey.keycode);
            KeySym sym = XLookupKeysym(&event.xkey, 0);
            Key key = native::nativeToKey(sym);
            Modifier modifier = modifier::modifierFromKey(key);
            if(modifier != NoModifier) pressed_modifiers_ &= ~modifier;
            int modifiers = pressed_modifiers_;
            dispatch_event = new KeyReleaseEvent(key, modifiers);
        }
        else if(event.type == MotionNotify)
        {
            if(!this->cursorVisible())
            {
                dispatch_event = new MouseMoveEvent(event.xmotion.x - this->size().width() / 2, event.xmotion.y - this->size().height() / 2, pressed_modifiers_);
                XSync(display_, False);
                XWarpPointer(display_, window_, window_, 0, 0, this->size().width(), this->size().height(),
                             this->size().width() / 2, this->size().height() / 2);
                XSync(display_, False);
                skip.push_back(XEventsQueued(display_, QueuedAlready));
            }
            else
            {
                float x = event.xmotion.x - (this->size_.width() / 2);
                float y = (this->size_.height() / 2) - event.xmotion.y;
                dispatch_event = new MouseMoveEvent(x, y, pressed_modifiers_);
            }
        }
        else if(event.type == ButtonPress)
        {
            float x, y;
            if(!this->cursorVisible())
            {
                x = event.xbutton.x - this->size().width() / 2;
                y = event.xbutton.y - this->size().height() / 2;
            }
            else
            {
                x = event.xbutton.x - (this->size_.width() / 2);
                y = (this->size_.height() / 2) - event.xbutton.y;
            }
            dispatch_event = new MousePressEvent(x, y, (MouseButton)event.xbutton.button, pressed_modifiers_);
        }
        else if(event.type == ButtonRelease)
        {
            float x, y;
            if(!this->cursorVisible())
            {
                x = event.xbutton.x - this->size().width() / 2;
                y = event.xbutton.y - this->size().height() / 2;
            }
            else
            {
                x = event.xbutton.x - (this->size_.width() / 2);
                y = (this->size_.height() / 2) - event.xbutton.y;
            }
            dispatch_event = new MouseReleaseEvent(x, y, (MouseButton)event.xbutton.button, pressed_modifiers_);
        }

        if(dispatch_event != nullptr)
        {
            if(this->subscriber_ == nullptr) log() - Debug < "No subscriber set, ignoring event";
            else if(this->event_handler_ == nullptr) log() - Debug < "Not event handler set, ignoring event";
            else (this->subscriber_->*this->event_handler_)(dispatch_event);

            delete dispatch_event;
            dispatch_event = nullptr;
        }

        for(unsigned int &i : skip)
            i--;
    }
}

template <class T>
void X11Window<T>::swapBuffers()
{
    glXSwapBuffers(display_, window_);
}

template<class T>
void X11Window<T>::windowStateChange(XConfigureEvent event)
{
    if(event.height != this->size_.height() || event.width != this->size_.width())
    {
        this->size_ = Size(event.width, event.height);
        if(this->subscriber_ != nullptr && this->viewport_change_handler_ != nullptr)
            (this->subscriber_->*this->viewport_change_handler_)(this->size_);
    }
}

template<class T>
void X11Window<T>::setCursorVisibility(bool visible)
{
    AbstractNativeWindow<T>::setCursorVisibility(visible);
    if(!visible)
    {
        XColor color = { 0, 0, 0, 0, 0, 0 };
        const char data[] = { 0 };

        Pixmap pixmap = XCreateBitmapFromData(display_, window_, data, 1, 1);
        Cursor cursor = XCreatePixmapCursor(display_, pixmap, pixmap, &color, &color, 0, 0);

        XFreePixmap(display_, pixmap);
        XDefineCursor(display_, window_, cursor);
        if(cursor_ != None) XFreeCursor(display_, cursor_);
        cursor_ = cursor;
    }
    else
    {
        XUndefineCursor(display_, window_);
        if(cursor_ != None)
        {
            XFreeCursor(display_, cursor_);
            cursor_ = None;
        }
    }
}

template <class T>
X11Window<T>::~X11Window()
{
    glXDestroyContext(display_, context_);
    XDestroyWindow(display_, window_);
    XFreeColormap(display_, colormap_);
    XCloseDisplay(display_);
    gladLoaderUnloadGLX();
}
