#ifndef NGE_OBJECT_HPP
#define NGE_OBJECT_HPP

#include "abstractions/abstract_mesh_component.hpp"
#include <vector>
#include <string>
#include "../enums/component_type.hpp"
#include "../data_objects/vec.hpp"
#include "../data_objects/rotation.hpp"
#include "abstractions/abstract_input_component.hpp"
#include "../data_objects/motion_vector.hpp"
#include "../data_objects/events/mouse_move_event.hpp"
#include "../data_objects/positioned_box.hpp"

class WorldStage;
class AbstractComponent;

class Object
{
public:
    explicit Object(std::string name);

    void addComponent(AbstractComponent *component);
    void removeComponent(const std::string &name);
    [[nodiscard]] int features() const;

    [[nodiscard]] const Vec3 &position() const;
    void setPosition(const Vec3 &position);
    [[nodiscard]] const Rotation &rotation() const;
    void setRotation(const Rotation &rotation);
    WorldStage *stage() const;
    void setStage(WorldStage *stage);
    MotionVector &motionVector();
    PositionedBox boundingBox() const;
    int collisionDamage() const;
    void setCollisionDamage(int collision_damage);
    const std::string &name() const;
    int userTypes() const;
    void setUserTypes(int types);

    virtual void load();
    virtual void unload();
    virtual void render();

    void keyPressEvent(KeyPressEvent *event);
    void keyReleaseEvent(KeyReleaseEvent *event);
    void mouseMoveEvent(MouseMoveEvent *event);

    virtual ~Object();

private:
    template<class T>
    static T *removeComponentFromVector(std::vector<T *> &vector, const std::string& name);

    std::vector<AbstractComponent*> components_;
    std::vector<AbstractMeshComponent*> mesh_components_;
    std::vector<AbstractInputComponent*> input_components_;

    int features_;
    int collision_damage_;
    int user_types_;
    std::string name_;

    Vec3 position_;
    Rotation rotation_;
    MotionVector motion_vector_;
    WorldStage *stage_;
};

#include "object.inl"

#endif //NGE_OBJECT_HPP
