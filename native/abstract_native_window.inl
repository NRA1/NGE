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
