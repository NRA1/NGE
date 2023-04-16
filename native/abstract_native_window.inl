template<class T>
AbstractNativeWindow<T>::AbstractNativeWindow(const std::string &title, Size size)
        : title_(title), size_(size),
          should_close_(false)
{

}

template<class T>
const Size &AbstractNativeWindow<T>::size() const
{
    return size_;
}

template <class T>
bool AbstractNativeWindow<T>::shouldClose()
{
    return should_close_;
}

template<class T>
void AbstractNativeWindow<T>::setShouldClose(bool should_close)
{
    should_close_ = should_close;
}

template<class T>
void AbstractNativeWindow<T>::setSubscriber(T *subscriber)
{
    subscriber_ = subscriber;
}

template <class T>
void AbstractNativeWindow<T>::setViewportChangeHandler(void (T::*handler)(Size))
{
    viewport_change_handler_ = handler;
}

template<class T>
void AbstractNativeWindow<T>::setEventHandler(void (T::*handler)(Event *))
{
    event_handler_ = handler;
}

template<class T>
bool AbstractNativeWindow<T>::cursorVisible() const
{
    return cursor_visible_;
}

template<class T>
void AbstractNativeWindow<T>::setCursorVisibility(bool visible)
{
    cursor_visible_ = visible;
}
