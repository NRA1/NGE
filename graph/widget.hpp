#ifndef NGE_WIDGET_HPP
#define NGE_WIDGET_HPP


#include "../data_objects/vec.hpp"
#include "../data_objects/rect.hpp"
#include "../data_objects/shader_program.hpp"
#include "../data_objects/events/key_press_event.hpp"
#include "../data_objects/events/key_release_event.hpp"
#include "../data_objects/events/mouse_move_event.hpp"

class Widget
{
public:
    Widget();

    const Vec2 &pos() const;
    void setPos(const Vec2 &pos);

    virtual Rect boundingRect() const = 0;

    virtual void load();
    virtual void render(std::map<unsigned int, ShaderProgram *> &shader_programs) const;
    virtual void unload();

    virtual bool keyPressEvent(KeyPressEvent *);
    virtual bool keyReleaseEvent(KeyReleaseEvent *);
    virtual bool mouseMoveEvent(MouseMoveEvent *);
    virtual void mouseEnterEvent();
    virtual void mouseLeaveEvent();

    virtual unsigned int requiredShaderPrograms() const;

    virtual ~Widget() = default;
private:
    Vec2 pos_;
};


#endif //NGE_WIDGET_HPP
