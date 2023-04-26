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
    bool handleEvent(Event *event) override;
    void render(unsigned int time) override;

    void addObject(Object *object);
    void removeObject(Object *object);
    void setCamera(Camera *camera);
    void setGround(Ground *ground);
    void setCollisionDelegate(bool (*collision_delegate)(WorldStage *, Object *, Object *));
    void setInputDelegate(bool (*input_delegate)(WorldStage *, Event *));
    void setOffworldDelegate(bool (*offworld_delegate)(WorldStage *, Object *));
    bool freeze() const;
    void setFreeze(bool freeze);
    void setTrackFilePath(std::optional<std::string> path);

    Object *findObjectByName(const std::string &name) const;
    std::vector<Object*> findObjectsByUserType(int type) const;

    ShaderProgram *shaderProgram();

    void onAppearing() override;
    void onDisappearing() override;

    void setTrackObject(const std::optional<std::string> &object);

    ~WorldStage() override;

    Camera *camera();

protected:
    Camera *camera_;
    Ground *ground_;
    ShaderProgram *shader_program_;
    bool visible_;

private:
    void applyPhysics(unsigned int time);

    Size viewport_;

    std::vector<Object *> objects_;

    Object *tracked_object_;
    std::optional<std::string> track_file_path_;
    std::optional<std::ofstream> track_file_;

    bool freeze_;

#if DISPLAY_COLLIDERS
    DebugBox *collider_display_;
    std::vector<PositionedBox> collider_boxes_;
#endif

    bool (*collision_delegate_)(WorldStage *stage, Object *object, Object *collider);
    bool (*input_delegate_)(WorldStage *stage, Event *event);
    bool (*offwold_delegate_)(WorldStage *stage, Object *event);

    friend class SaveManager;
};


#endif //NGE_WORLD_STAGE_HPP
