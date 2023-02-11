#ifndef NGE_EVENT_HPP
#define NGE_EVENT_HPP


class Event
{
public:
    [[nodiscard]] virtual int type() const = 0;
    virtual ~Event() = default;
};


#endif //NGE_EVENT_HPP
