#include <ft2build.h>
#include FT_FREETYPE_H
#include "infrastructure/logger.hpp"
#include "infrastructure/game.hpp"
#include "data_objects/size.hpp"
#include "interface/game_window.hpp"
#include "interface/world_stage.hpp"
#include "world/object.hpp"
#include "world/mesh_component.hpp"
#include "world/input_component.hpp"
#include "world/debug_box.hpp"
#include "game/npc_component.hpp"
#include "interface/graph_stage.hpp"
#include "graph/rect_widget.hpp"
#include "graph/text_widget.hpp"
#include "world/bar_component.h"
#include "world/property_component.h"

void run();

int enemies = 3;
int arenas = 1;

TextWidget *enemies_widget;
TextWidget *arenas_widget;

namespace delegates
{
    unsigned int bullet_counter = 0;

    bool collisionDelegate(WorldStage *stage, Object *object, Object *collider)
    {
        if(object->name().starts_with("bullet"))
        {
            auto adamage = object->findComponent("damage");
            if (adamage != nullptr && adamage->type() & PropertyComponentType)
            {
                auto alife = collider->findComponent("life");
                if (alife != nullptr && alife->type() & BarComponentType)
                {
                    BarComponent *life = (BarComponent *) alife;
                    PropertyComponent<unsigned int> *damage = (PropertyComponent<unsigned int> *) adamage;
                    if (life->value() <= damage->value())
                    {
                        if(collider->name().starts_with("arena"))
                        {
                            arenas--;
                            arenas_widget->setText("Arenas: " + std::to_string(arenas));
                        }
                        else if(collider->name().starts_with("enemy"))
                        {
                            enemies--;
                            enemies_widget->setText("Enemies: " + std::to_string(enemies));
                        }
                        stage->removeObject(collider);
                    }
                    else
                        life->adjustValue(-((int)damage->value()));

                    stage->removeObject(object);
                    return true;
                }
            }
        }
        return true;
    }

    bool inputDelegate(WorldStage *stage, Event *event)
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
                    stage->camera()->setRotation(Rotation(30, -90, 90));
                    stage->camera()->setPosition(Vec3(300, 500, 300));
                    stage->camera()->setTargetObject(nullptr);
                    mmo = stage->camera()->motionVector()->generateManipulationObject(true);
                    camera_tracks = false;
                }
                else
                {
                    Object *player = stage->findObjectByName("player");
                    if(player == nullptr) return true;
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
                if(player == nullptr) return true;
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
                if(player == nullptr) return true;
                player->mouseMoveEvent(ev);
            }
        }
        else if(event->type() == MousePressEventType)
        {
            Object *player = stage->findObjectByName("player");
            if(player == nullptr) return false;
            Mat4 mat = Mat4(1);
            mat = glm::rotate(mat, glm::radians(360 - player->rotation().roll()), Vec3(0, 1, 0));
            Vec4 rel_start_pos = Vec4(0, 100, 50, 0) * mat;
            Object *bullet = new Object("bullet" + std::to_string(bullet_counter++));
            bullet->setPosition(Vec3(player->position().x + rel_start_pos.x, player->position().y + rel_start_pos.y,
                                     player->position().z + rel_start_pos.z));
            MeshComponent *mesh_comp = new MeshComponent("mesh", ":/models/bullet/bullet.obj");
            bullet->addComponent(mesh_comp);
            PropertyComponent<unsigned int> *damage_component = new PropertyComponent<unsigned int>("damage", 30);
            bullet->addComponent(damage_component);
            stage->addObject(bullet);
            auto mvo = bullet->motionVector().generateManipulationObject(false);
            Vec4 rotated = Vec4(0, 0, 200, 0) * mat;
            mvo->setVelocity(rotated);
        }
        return true;
    }

    bool offworldDelegate(WorldStage *stage, Object *obj)
    {
        stage->removeObject(obj);
        return true;
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

int APIENTRY WinMain(_In_ HINSTANCE hinst, _In_opt_ HINSTANCE hinstprev, _In_ PSTR cmdline, _In_ int cmdshow)
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
        for(int i = 0; i < enemies; i++)
        {
            Object *enemy = new Object("enemy" + std::to_string(i));
            MeshComponent *enemy_comp = new MeshComponent("mesh", ":/models/bot/bot4.obj");
            NPCComponent *enemy_npc = new NPCComponent("npc");
            BarComponent *enemy_life = new BarComponent("life", 50, 50);
            enemy->setPosition(Vec3(100 * (i + 2), 0, 100 * (1 + i)));
            enemy->addComponent(enemy_comp);
            enemy->addComponent(enemy_npc);
            enemy->addComponent(enemy_life);
            stage->addObject(enemy);
        }
        Object *arena = new Object("arena0");
        MeshComponent *arena_comp = new MeshComponent("mesh", ":/models/arena/arena.fbx");
        arena->addComponent(arena_comp);
        BarComponent *arena_life_bar = new BarComponent("life", 100, 100);
        arena->addComponent(arena_life_bar);
        arena->setPosition(Vec3(600, 0, -600));
        stage->addObject(arena);
        stage->setCamera(camera);
        stage->setCollisionDelegate(&delegates::collisionDelegate);
        stage->setInputDelegate(&delegates::inputDelegate);
        stage->setOffworldDelegate(&delegates::offworldDelegate);
        stage->setGround(ground);
        GraphStage *graph = new GraphStage();
        enemies_widget = new TextWidget("Enemies: " + std::to_string(enemies), ":/fonts/arial.ttf", 20, Vec4(1, 1, 1, 1));
        arenas_widget = new TextWidget("Arenas: " + std::to_string(arenas), ":/fonts/arial.ttf", 20, Vec4(1, 1, 1, 1));
        enemies_widget->setPos(Vec2(700, 500));
        arenas_widget->setPos(Vec2(700, 470));
        graph->addWidget(enemies_widget);
        graph->addWidget(arenas_widget);
//        RectWidget *widget = new RectWidget(Rect(-100, -100, 200, 200), Vec4(1, 1, 1, 1));
//        widget->setPos(Vec2(300, 300));
//        graph->addWidget(widget);
//        TextWidget *text = new TextWidget("Some random text", ":/fonts/arial.ttf", 100, Vec4(1, 1, 1, 1));
//        text->setPos(Vec2(0, 0));
//        graph->addWidget(text);
        window->setCursorVisibility(false);
        window->pushStage(stage);
        window->pushStage(graph);
        window->exec();
        delete window;
        Game::shutdown();
    }
    catch (const char *e)
    {
        log() - Fatal < e <<= trace;
    }
}