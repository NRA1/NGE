#ifndef NGE_OBJECT_HPP
#define NGE_OBJECT_HPP


#include <vector>
#include "../enums/capability.hpp"
#include "../data_objects/vec.hpp"
#include "abstract_component.hpp"

class AbstractComponent;
class WorldStage;

class Object
{
public:
    explicit Object();

    void addComponent(AbstractComponent *component);
    void removeComponent(const std::string &name);
    [[nodiscard]] int capabilities() const;

    [[nodiscard]] const Vec3 &position() const;
    void setPosition(const Vec3 &position);
    [[nodiscard]] const Vec3 &rotation() const;
    void setRotation(const Vec3 &rotation);
    WorldStage *stage() const;
    void setStage(WorldStage *stage);

    void load();
    void unload();
    void render();

    ~Object();

private:
    static AbstractComponent *removeComponentFromVector(std::vector<AbstractComponent *> &vector, const std::string& name);

    std::vector<AbstractComponent*> components_;
    std::vector<AbstractComponent*> mesh_components_;

    int capabilities_;

    Vec3 position_;
    Vec3 rotation_;
    WorldStage *stage_;
};


#endif //NGE_OBJECT_HPP
