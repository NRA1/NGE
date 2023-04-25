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
#include "graph/button_widget.hpp"
#include "enums/layout_origin.hpp"
#include "enums/layout_flags.hpp"
#include "game/game_component_type.hpp"
#include "interface/replay_stage.hpp"

void run();

int enemies = 3;
int arenas = 1;

GameWindow *window;
WorldStage *world_stage;
ReplayStage *replay_stage = nullptr;

TextWidget *enemies_widget;
TextWidget *arenas_widget;

RectWidget *background;
ButtonWidget *replay_btn;
ButtonWidget *save_btn;
ButtonWidget *load_btn;
ButtonWidget *exit_btn;
ButtonWidget *return_btn;


namespace delegates
{
    unsigned int bullet_counter = 0;
    bool menu_visible = false;

    bool collisionDelegate(WorldStage *stage, Object *object, Object *collider)
    {
        if (object->name().starts_with("bullet"))
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
                        if (collider->name().starts_with("arena"))
                        {
                            arenas--;
                            arenas_widget->setText("Arenas: " + std::to_string(arenas));
                        }
                        else if (collider->name().starts_with("enemy"))
                        {
                            enemies--;
                            enemies_widget->setText("Enemies: " + std::to_string(enemies));
                        }
                        stage->removeObject(collider);
                    }
                    else
                        life->adjustValue(-((int) damage->value()));

                    stage->removeObject(object);
                    return true;
                }
            }
        }
        return true;
    }

    bool inputDelegate(WorldStage *stage, Event *event)
    {
        if (event->type() == KeyPressEventType)
        {
            KeyPressEvent *ev = (KeyPressEvent *) event;

            if (ev->key() == Key::KeyEscape)
            {
                menu_visible = !menu_visible;
                window->setCursorVisibility(menu_visible);
                stage->setFreeze(menu_visible);
                background->setVisible(menu_visible);
                replay_btn->setVisible(menu_visible);
                save_btn->setVisible(menu_visible);
                load_btn->setVisible(menu_visible);
                exit_btn->setVisible(menu_visible);

                return true;
            }
            if (!menu_visible)
            {
                Object *player = stage->findObjectByName("player");
                if (player == nullptr) return true;
                player->keyPressEvent(ev);
            }
        }
        else if (event->type() == KeyReleaseEventType)
        {
            if (!menu_visible)
            {
                KeyReleaseEvent *ev = (KeyReleaseEvent *) event;
                Object *player = stage->findObjectByName("player");
                if (player == nullptr) return true;
                player->keyReleaseEvent(ev);
            }
        }
        else if (event->type() == MouseMoveEventType)
        {
            if (!menu_visible)
            {
                MouseMoveEvent *ev = (MouseMoveEvent *) event;

                Object *player = stage->findObjectByName("player");
                if (player == nullptr) return true;
                player->mouseMoveEvent(ev);
            }
        }
        else if (event->type() == MousePressEventType)
        {
            if (!menu_visible)
            {
                Object *player = stage->findObjectByName("player");
                if (player == nullptr) return false;
                Mat4 mat = Mat4(1);
                mat = glm::rotate(mat, glm::radians(360 - player->rotation().roll()), Vec3(0, 1, 0));
                Vec4 rel_start_pos = Vec4(0, 50, 50, 0) * mat;
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
        }
        return true;
    }

    bool offworldDelegate(WorldStage *stage, Object *obj)
    {
        stage->removeObject(obj);
        return true;
    }

    bool replayInputDelegate(ReplayStage *stage, Event *event)
    {
        if (event->type() == KeyPressEventType)
        {
            KeyPressEvent *ev = (KeyPressEvent *) event;

            if (ev->key() == Key::KeyEscape)
            {
                menu_visible = !menu_visible;
                window->setCursorVisibility(menu_visible);
                stage->setFreeze(menu_visible);
                background->setVisible(menu_visible);
                return_btn->setVisible(menu_visible);
                exit_btn->setVisible(menu_visible);

                return true;
            }
        }
        return false;
    }

    AbstractComponent *componentLoader(unsigned int type, std::ifstream &ifs)
    {
        if (type == ComponentType::BarComponentType) return new BarComponent(ifs);
        else if (type == ComponentType::InputComponentType) return new InputComponent(ifs);
        else if (type == ComponentType::MeshComponentType) return new MeshComponent(ifs);
        else if (type == ComponentType::PropertyComponentType) return new PropertyComponent<unsigned int>(ifs);
        else if (type == GameComponentType::NPCComponentType) return new NPCComponent(ifs);
        throw "Unknown type";
    }

    bool save_btn_handler()
    {
        try
        {
            SaveManager::dump(world_stage, ":/save/save1.dat");
        }
        catch (char *err)
        {
            log() - Critical < "Failed to save:" < err;
        }
        return true;
    }

    bool load_btn_handler()
    {
        try
        {
            WorldStage *world_stage_t = SaveManager::load(":/save/save1.dat");
            world_stage_t->setCollisionDelegate(&delegates::collisionDelegate);
            world_stage_t->setInputDelegate(&delegates::inputDelegate);
            world_stage_t->setOffworldDelegate(&delegates::offworldDelegate);
            Object *player = world_stage_t->findObjectByName("player");
            world_stage_t->camera()->setTargetObject(player);
            world_stage_t->setFreeze(true);
            window->eraseStage(world_stage);
            delete world_stage;
            world_stage = world_stage_t;
            window->insertStage(world_stage, 0);
        }
        catch (const char *err)
        {
            log() - Critical < "Failed to load save:" < err;
        }
        return true;
    }

    bool exit_btn_handler()
    {
        window->close();
        return true;
    }

    void replay_finished_handler(ReplayStage *)
    {
        menu_visible = true;
        window->setCursorVisibility(menu_visible);
        background->setVisible(menu_visible);
        return_btn->setVisible(menu_visible);
        exit_btn->setVisible(menu_visible);
    }

    bool replay_btn_handler()
    {
        replay_stage = new ReplayStage(":/trackings/track1.dat");
        Object *object = new Object("player");
        MeshComponent *component = new MeshComponent("mesh", ":/models/bot/bot4.obj");
        object->addComponent(component);
        replay_stage->setObject(object);
        replay_stage->setInputDelegate(delegates::replayInputDelegate);
        replay_stage->setFinishedDelegate(delegates::replay_finished_handler);
        replay_stage->setFreeze(true);

        Ground *ground = new Ground(Rect(-1000, -1000, 2000, 2000), ":/models/ground/color.jpg");
        Camera *camera = new Camera();
        replay_stage->setGround(ground);
        replay_stage->setCamera(camera);
        camera->setTargetObject(object);
        camera->setPosition(Vec3(0.0f, 300.0f, -200.0f));

        window->eraseStage(world_stage);
        window->insertStage(replay_stage, 0);
        return_btn->setVisible(true);
        replay_btn->setVisible(false);
        save_btn->setVisible(false);
        load_btn->setVisible(false);
        return true;
    }

    bool return_btn_handler()
    {
        window->insertStage(world_stage, 0);
        window->eraseStage(replay_stage);
        delete replay_stage;
        replay_stage = nullptr;
        replay_btn->setVisible(true);
        return_btn->setVisible(false);
        save_btn->setVisible(true);
        load_btn->setVisible(true);

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
        SaveManager::setComponentLoader(&delegates::componentLoader);
        window = new GameWindow("NGE", 1000, 700, 0);
        world_stage = new WorldStage();
        Object *object = new Object("player");
        Camera *camera = new Camera();
        camera->setTargetObject(object);
        camera->setPosition(Vec3(0.0f, 300.0f, -200.0f));
        MeshComponent *component = new MeshComponent("mesh", ":/models/bot/bot4.obj");
        Ground *ground = new Ground(Rect(-1000, -1000, 2000, 2000), ":/models/ground/color.jpg");
        object->addComponent(component);
        InputComponent *input = new InputComponent("input");
        object->addComponent(input);
        world_stage->addObject(object);
        world_stage->setTrackFilePath(":/trackings/track1.dat");
        world_stage->setTrackObject("player");
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
            world_stage->addObject(enemy);
        }
        Object *arena = new Object("arena0");
        MeshComponent *arena_comp = new MeshComponent("mesh", ":/models/arena/arena.fbx");
        arena->addComponent(arena_comp);
        BarComponent *arena_life_bar = new BarComponent("life", 100, 100);
        arena->addComponent(arena_life_bar);
        arena->setPosition(Vec3(600, 0, -600));
        world_stage->addObject(arena);
        world_stage->setCamera(camera);
        world_stage->setCollisionDelegate(&delegates::collisionDelegate);
        world_stage->setInputDelegate(&delegates::inputDelegate);
        world_stage->setOffworldDelegate(&delegates::offworldDelegate);
        world_stage->setGround(ground);
        GraphStage *graph = new GraphStage();
        enemies_widget = new TextWidget("Enemies: " + std::to_string(enemies), ":/fonts/arial.ttf", 20, Vec4(1, 1, 1, 1));
        arenas_widget = new TextWidget("Arenas: " + std::to_string(arenas), ":/fonts/arial.ttf", 20, Vec4(1, 1, 1, 1));
        enemies_widget->setPos(Vec2(-130, -40));
        arenas_widget->setPos(Vec2(-130, -80));
        enemies_widget->setLayoutOrigin(LayoutOrigin::Top | LayoutOrigin::Right);
        arenas_widget->setLayoutOrigin(LayoutOrigin::Top | LayoutOrigin::Right);
        arenas_widget->setZPos(-0.7);
        enemies_widget->setZPos(-0.7);
        graph->addWidget(enemies_widget);
        graph->addWidget(arenas_widget);
        background = new RectWidget(Rect(0, 0, 100, 100), Vec4(0, 0, 0, 0.5));
        background->setLayoutOrigin(LayoutOrigin::Bottom | LayoutOrigin::Left);
        background->setLayoutFlags(LayoutFlags::SizeRelative);
        background->setZPos(-0.3);
        background->setVisible(false);
        graph->addWidget(background);

        replay_btn = new ButtonWidget("Replay", Rect(-150, 95, 300, 50), &delegates::replay_btn_handler,
                                      Vec4(1, 1, 1, 1), Vec4(0, 0, 0, 1), 40);
        replay_btn->setZPos(-0.5);
        replay_btn->setVisible(false);

        return_btn = new ButtonWidget("Return", Rect(-150, -25, 300, 50), &delegates::return_btn_handler,
                                      Vec4(1, 1, 1, 1), Vec4(0, 0, 0, 1), 40);
        return_btn->setZPos(-0.5);
        return_btn->setVisible(false);

        save_btn = new ButtonWidget("Save", Rect(-150, 35, 300, 50), &delegates::save_btn_handler,
                                                  Vec4(1, 1, 1, 1), Vec4(0, 0, 0, 1), 40);
        save_btn->setZPos(-0.5);
        save_btn->setVisible(false);

        load_btn = new ButtonWidget("Load", Rect(-150, -25, 300, 50), &delegates::load_btn_handler,
                                    Vec4(1, 1, 1, 1), Vec4(0, 0, 0, 1), 40);
        load_btn->setZPos(-0.5);
        load_btn->setVisible(false);

        exit_btn = new ButtonWidget("Exit", Rect(-150, -85, 300, 50), &delegates::exit_btn_handler,
                                                  Vec4(1, 1, 1, 1), Vec4(0, 0, 0, 1), 40);
        exit_btn->setZPos(-0.5);
        exit_btn->setVisible(false);
        graph->addWidget(replay_btn);
        graph->addWidget(save_btn);
        graph->addWidget(load_btn);
        graph->addWidget(return_btn);
        graph->addWidget(exit_btn);
        window->setCursorVisibility(false);
        window->pushStage(world_stage);
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