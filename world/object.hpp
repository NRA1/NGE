#ifndef NGE_OBJECT_HPP
#define NGE_OBJECT_HPP

#include "abstractions/abstract_mesh_component.hpp"
#include <vector>
#include <string>
#include "../enums/component_type.hpp"
#include "../data_objects/vec.hpp"

class WorldStage;
class AbstractComponent;

class Object
{
public:
    explicit Object();

    void addComponent(AbstractComponent *component);
    void removeComponent(const std::string &name);
    [[nodiscard]] int features() const;

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
    template<class T>
    static T *removeComponentFromVector(std::vector<T *> &vector, const std::string& name);

    std::vector<AbstractComponent*> components_;
    std::vector<AbstractMeshComponent*> mesh_components_;

    int features_;

    Vec3 position_;
    Vec3 rotation_;
    WorldStage *stage_;
};

#include "object.inl"

#endif //NGE_OBJECT_HPP
