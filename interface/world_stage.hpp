#ifndef NGE_WORLD_STAGE_HPP
#define NGE_WORLD_STAGE_HPP

#include <glad/gl.h>
#include <vector>

#include "abstract_stage.hpp"
#include "../data_objects/rect.hpp"
#include "../world/object.hpp"
#include "../world/camera.hpp"
#include "../world/object.hpp"
#include "../data_objects/events/event.hpp"
#include "../data_objects/events/key_press_event.hpp"
#include "../enums/event_type.hpp"
#include "../enums/key.hpp"

class Object;

class WorldStage : public AbstractStage
{
public:
    WorldStage();

    void viewportSizeChanged(Size size) override;
    void handleEvent(Event *event) override;
    void render() override;

    void addObject(Object *object);
    void removeObject(Object *object);
    void setCamera(Camera *camera);

    ShaderProgram *shaderProgram();

    void onAppearing() override;
    void onDisappearing() override;

    ~WorldStage() override;

private:
    Size viewport_;

    std::vector<Object *> objects_;
    Camera *camera_;
    ShaderProgram *shader_program_;
};


#endif //NGE_WORLD_STAGE_HPP
