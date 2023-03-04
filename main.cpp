#include "infrastructure/logger.hpp"
#include "infrastructure/game.hpp"
#include "data_objects/size.hpp"
#include "interface/game_window.hpp"
#include "interface/world_stage.hpp"
#include "world/object.hpp"
#include "world/mesh_component.hpp"
#include "world/input_component.hpp"
#include "world/debug_box.hpp"

void run();

namespace delegates
{
    bool collisionDelegate(WorldStage *, Object *, Object *)
    {
//        stage->removeObject(collider);
        return true;
    }

    void inputDelegate(WorldStage *stage, Event *event)
    {
        static bool camera_tracks = true;
        static MotionManipulationObject *mmo = nullptr;
        if(event->type() == KeyPressEventType)
        {
            KeyPressEvent *ev = (KeyPressEvent*)event;
            if(camera_tracks)
            {
                if (ev->key() == Key::KeySpace)
                {
//                    stage->camera()->setRotation(Rotation(30, -90, 90));
//                    stage->camera()->setPosition(Vec3(300, 500, 300));
//                    stage->camera()->setTargetObject(nullptr);
//                    mmo = stage->camera()->motionVector()->generateManipulationObject(true);
//                    camera_tracks = false;
                }
                else
                {
                    Object *player = stage->findObjectByName("player");
                    player->keyPressEvent(ev);
                }
            }
            else
            {
                if(ev->key() == Key::KeySpace)
                {
                    stage->camera()->motionVector()->destroyManipulationObject(mmo);
                    mmo = nullptr;
                    stage->camera()->setTargetObject(stage->findObjectByName("player"));
                    camera_tracks = true;
                }
                if(!ev->isRepeated())
                {
                    if(ev->key() == Key::KeyLeft)
                    {
                        std::optional<Rotation> rot = mmo->rotation();
                        if(!rot.has_value()) rot = Rotation(0, 0, 0);
                        mmo->setRotation(rot.value() + Rotation(5, 0, 0));
                    }
                    else if(ev->key() == Key::KeyRight)
                    {
                        std::optional<Rotation> rot = mmo->rotation();
                        if(!rot.has_value()) rot = Rotation(0, 0, 0);
                        mmo->setRotation(rot.value() + Rotation(-5, 0, 0));
                    }
                    else if(ev->key() == Key::KeyUp)
                    {
                        std::optional<Rotation> rot = mmo->rotation();
                        if(!rot.has_value()) rot = Rotation(0, 0, 0);
                        mmo->setRotation(rot.value() + Rotation(0, 0, 5));
                    }
                    else if(ev->key() == Key::KeyDown)
                    {
                        std::optional<Rotation> rot = mmo->rotation();
                        if(!rot.has_value()) rot = Rotation(0, 0, 0);
                        mmo->setRotation(rot.value() + Rotation(0, 0, -5));
                    }
                }
            }
        }
        else if(event->type() == KeyReleaseEventType)
        {
            KeyReleaseEvent *ev = (KeyReleaseEvent*)event;
            if(camera_tracks)
            {
                Object *player = stage->findObjectByName("player");
                player->keyReleaseEvent(ev);
            }
            else
            {
                if(ev->key() == Key::KeyLeft)
                {
                    std::optional<Rotation> rot = mmo->rotation();
                    if(!rot.has_value()) rot = Rotation(0, 0, 0);
                    mmo->setRotation(rot.value() + Rotation(-5, 0, 0));
                }
                else if(ev->key() == Key::KeyRight)
                {
                    std::optional<Rotation> rot = mmo->rotation();
                    if(!rot.has_value()) rot = Rotation(0, 0, 0);
                    mmo->setRotation(rot.value() + Rotation(5, 0, 0));
                }
                else if(ev->key() == Key::KeyUp)
                {
                    std::optional<Rotation> rot = mmo->rotation();
                    if(!rot.has_value()) rot = Rotation(0, 0, 0);
                    mmo->setRotation(rot.value() + Rotation(0, 0, -5));
                }
                else if(ev->key() == Key::KeyDown)
                {
                    std::optional<Rotation> rot = mmo->rotation();
                    if(!rot.has_value()) rot = Rotation(0, 0, 0);
                    mmo->setRotation(rot.value() + Rotation(0, 0, 5));
                }
            }
        }
        else if(event->type() == MouseMoveEventType)
        {
            MouseMoveEvent *ev = (MouseMoveEvent*)event;
            if(camera_tracks)
            {
                Object *player = stage->findObjectByName("player");
                player->mouseMoveEvent(ev);
            }
        }
    }
}

#ifdef __unix__
int main()
{
    Game::initialize();
    run();
}
#endif
#ifdef _WIN32
#include <windows.h>

int APIENTRY WinMain(HINSTANCE hinst, HINSTANCE hinstprev, PSTR cmdline, int cmdshow)
{
    WinmainConfig config;
    config.Instance = hinst;
    config.PrevInstance = hinstprev;
    config.CmdLine = cmdline;
    config.ShowCmd = cmdshow;

    Game::initialize(config);
    run();
}
#endif

void run()
{
    try
    {
        GameWindow *window = new GameWindow("NGE", 1000, 700, 0);
        WorldStage *stage = new WorldStage();
        Object *object = new Object("player");
        Camera *camera = new Camera();
        camera->setTargetObject(object);
        camera->setPosition(Vec3(0.0f, 300.0f, -200.0f));
        MeshComponent *component = new MeshComponent("mesh", ":/models/bot/bot4.obj");
        Ground *ground = new Ground(Rect(-1000, -1000, 2000, 2000), ":/models/ground/color.jpg");
        object->addComponent(component);
        InputComponent *input = new InputComponent("input");
        object->addComponent(input);
        stage->addObject(object);
        Object *colider = new Object("tmpColliderObj");
        MeshComponent *colider_comp = new MeshComponent("mesh", ":/models/bot/bot4.obj");
        colider->setPosition(Vec3(200, 0, 200));
        colider->addComponent(colider_comp);
        stage->addObject(colider);
        Object *arena = new Object("arena");
        MeshComponent *arena_comp = new MeshComponent("mesh", ":/models/arena/arena.obj");
        arena->addComponent(arena_comp);
        arena->setPosition(Vec3(600, 0, -600));
        stage->addObject(arena);
        stage->setCamera(camera);
        stage->setCollisionDelegate(&delegates::collisionDelegate);
        stage->setInputDelegate(&delegates::inputDelegate);
        stage->setGround(ground);
        window->setCursorVisibility(false);
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