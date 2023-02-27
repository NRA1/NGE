#ifndef NGE_WORLD_STAGE_HPP
#define NGE_WORLD_STAGE_HPP

#include <glad/gl.h>
#include <vector>
#include <chrono>

#include "abstract_stage.hpp"
#include "../data_objects/rect.hpp"
#include "../world/object.hpp"
#include "../world/camera.hpp"
#include "../world/object.hpp"
#include "../data_objects/events/event.hpp"
#include "../data_objects/events/key_press_event.hpp"
#include "../enums/event_type.hpp"
#include "../enums/key.hpp"
#include "../world/ground.hpp"
#include "../interface/interface_helpers.hpp"
#include "../world/debug_box.hpp"

class Object;

class WorldStage : public AbstractStage
{
public:
    WorldStage();

    void viewportSizeChanged(Size size) override;
    void handleEvent(Event *event) override;
    void render(unsigned int time) override;

    void addObject(Object *object);
    void removeObject(Object *object);
    void setCamera(Camera *camera);
    void setGround(Ground *ground);
    void setCollisionDelegate(bool (*collision_delegate)(WorldStage *, Object *, Object *));
    void setInputDelegate(void (*inputDelegate)(WorldStage *, Event *));

    Object *findObjectByName(const std::string &name) const;

    ShaderProgram *shaderProgram();

    void onAppearing() override;
    void onDisappearing() override;

    ~WorldStage() override;

    Camera *camera();

private:
    void applyPhysics(unsigned int time);

    Size viewport_;

    std::vector<Object *> objects_;
    Camera *camera_;
    Ground *ground_;
    ShaderProgram *shader_program_;

#if DISPLAY_COLLIDERS
    DebugBox *collider_display_;
    std::vector<PositionedBox> collider_boxes_;
#endif

    bool (*collision_delegate_)(WorldStage *stage, Object *object, Object *collider);
    void (*input_delegate_)(WorldStage *stage, Event *event);
};


#endif //NGE_WORLD_STAGE_HPP
