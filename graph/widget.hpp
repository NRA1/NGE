#ifndef NGE_WIDGET_HPP
#define NGE_WIDGET_HPP


#include "../data_objects/vec.hpp"
#include "../data_objects/rect.hpp"
#include "../data_objects/shader_program.hpp"
#include "../data_objects/events/key_press_event.hpp"
#include "../data_objects/events/key_release_event.hpp"
#include "../data_objects/events/mouse_move_event.hpp"
#include "../data_objects/events/mouse_press_event.h"
#include "../data_objects/events/mouse_release_event.h"

class Widget
{
public:
    Widget();

    const Vec2 &pos() const;
    void setPos(const Vec2 &pos);

    float zPos() const;
    void setZPos(float z_pos);
    bool visible() const;
    void setVisible(bool visible);

    unsigned int layoutFlags() const;
    void setLayoutFlags(unsigned int layout_flags);
    unsigned int layoutOrigin() const;
    void setLayoutOrigin(unsigned int layout_origin);

    virtual Rect boundingRect() const = 0;

    virtual void load();
    virtual void render(std::map<unsigned int, ShaderProgram *> &shader_programs) const;
    virtual void unload();

    virtual bool keyPressEvent(KeyPressEvent *);
    virtual bool keyReleaseEvent(KeyReleaseEvent *);
    virtual bool mouseMoveEvent(MouseMoveEvent *);
    virtual void mouseEnterEvent();
    virtual void mouseLeaveEvent();
    virtual bool mousePressEvent(MousePressEvent *);
    virtual bool mouseReleaseEvent(MouseReleaseEvent *);

    virtual unsigned int requiredShaderPrograms() const;

    virtual ~Widget() = default;
private:
    Vec2 pos_;
    float z_pos_;

    bool visible_;

    unsigned int layout_flags_;
    unsigned int layout_origin_;
};


#endif //NGE_WIDGET_HPP
