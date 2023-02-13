#include "infrastructure/logger.hpp"
#include "infrastructure/game.hpp"
#include "data_objects/size.hpp"
#include "interface/window.hpp"
#include "interface/world_stage.hpp"
#include "world/object.hpp"
#include "world/mesh_component.hpp"

int main()
{
    try
    {
        Game::initialize();
        GameWindow *window = new GameWindow("Test", 1000, 700, 0);
        WorldStage *stage = new WorldStage();
        Object *object = new Object();
        Camera *camera = new Camera();
        camera->setTargetObject(object);
        MeshComponent *component = new MeshComponent("mesh", ":/models/ch1/CH1.fbx");
        Ground *ground = new Ground(Rect(-1000, -1000, 2000, 2000), ":/models/ground/diffuse.jpg");
        object->addComponent(component);
        stage->addObject(object);
        stage->setCamera(camera);
        stage->setGround(ground);
        window->setStage(stage);
        window->exec();
        delete window;
        Game::shutdown();
    }
    catch (const char *e)
    {
        log() - Fatal < e <<= trace;
    }
}