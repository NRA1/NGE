#include "main.hpp"
#include "game/user_object_type.hpp"

void *unuse(void *) { return (void*)0; };


bool delegates::collisionDelegate(WorldStage *stage, Object *object, Object *collider)
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
                        g_arenas--;
                        g_points += 3;
                        g_arenas_widget->setText("Arenas: " + std::to_string(g_arenas));
                        g_points_widget->setText("Points: " + std::to_string(g_points));
                    }
                    else if (collider->name().starts_with("enemy"))
                    {
                        g_enemies--;
                        g_points++;
                        g_enemies_widget->setText("Enemies: " + std::to_string(g_enemies));
                        g_points_widget->setText("Points: " + std::to_string(g_points));
                    }
                    stage->removeObject(collider);

                    if(g_enemies == 0 && g_arenas == 0)
                    {
                        g_points += 5;
                        g_points_widget->setText("Points: " + std::to_string(g_points));
                        g_world_stage->setFreeze(true);
                        setMenu(MenuMode::NextLevel);
                        setLevel(g_level + 1);
                        return true;
                    }
                }
                else
                    life->adjustValue(-((int) damage->value()));

                stage->removeObject(object);
                return true;
            }
        }
    }
    else if(object->name().starts_with("enemy") && collider->name() == "player")
    {
        stage->setFreeze(true);
        setMenu(MenuMode::Lost);
        return true;
    }
    return true;
}

bool delegates::inputDelegate(WorldStage *stage, Event *event)
{
    if (event->type() == KeyPressEventType)
    {
        KeyPressEvent *ev = (KeyPressEvent *) event;

        if (ev->key() == Key::KeyEscape)
        {
            setMenu(g_menu_mode == MenuMode::Play ? MenuMode::Hidden : MenuMode::Play);
            stage->setFreeze(g_menu_mode != MenuMode::Hidden);
            return true;
        }
        Object *player = stage->findObjectByName("player");
        if (player == nullptr) return true;
        player->keyPressEvent(ev);
    }
    else if (event->type() == KeyReleaseEventType)
    {
        KeyReleaseEvent *ev = (KeyReleaseEvent *) event;
        Object *player = stage->findObjectByName("player");
        if (player == nullptr) return true;
        player->keyReleaseEvent(ev);
    }
    else if (event->type() == MouseMoveEventType)
    {
        MouseMoveEvent *ev = (MouseMoveEvent *) event;

        Object *player = stage->findObjectByName("player");
        if (player == nullptr) return true;
        player->mouseMoveEvent(ev);
    }
    else if (event->type() == MousePressEventType)
    {
        Object *player = stage->findObjectByName("player");
        if (player == nullptr) return false;
        Mat4 mat = Mat4(1);
        mat = glm::rotate(mat, glm::radians(360 - player->rotation().roll()), Vec3(0, 1, 0));
        Vec4 rel_start_pos = Vec4(0, 50, 50, 0) * mat;
        Object *bullet = new Object("bullet" + std::to_string(bullet_counter++));
        bullet->setUserType(UserObjectType::BulletType);
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

bool delegates::offworldDelegate(WorldStage *stage, Object *obj)
{
    if (obj->name().starts_with("arena"))
    {
        g_arenas--;
        g_points += 3;
        g_arenas_widget->setText("Arenas: " + std::to_string(g_arenas));
        g_points_widget->setText("Points: " + std::to_string(g_points));
    }
    else if (obj->name().starts_with("enemy"))
    {
        g_enemies--;
        g_points++;
        g_enemies_widget->setText("Enemies: " + std::to_string(g_enemies));
        g_points_widget->setText("Points: " + std::to_string(g_points));
    }

    if(obj->name() == "player")
    {
        stage->setFreeze(true);
        setMenu(MenuMode::Lost);
    }

    stage->removeObject(obj);

    if(g_enemies == 0 && g_arenas == 0)
    {
        g_points += 5;
        g_points_widget->setText("Points: " + std::to_string(g_points));
        g_world_stage->setFreeze(true);
        setMenu(MenuMode::Play);
        setLevel(g_level + 1);
        return true;
    }

    return true;
}

bool delegates::replayInputDelegate(ReplayStage *stage, Event *event)
{
    if (event->type() == KeyPressEventType)
    {
        KeyPressEvent *ev = (KeyPressEvent *) event;

        if (ev->key() == Key::KeyEscape)
        {
            setMenu(g_menu_mode == MenuMode::Hidden ? MenuMode::Replay : MenuMode::Hidden);
            stage->setFreeze(g_menu_mode != MenuMode::Hidden);

            return true;
        }
    }
    return false;
}

bool delegates::graphInputDelegate(GraphStage *, Event *event)
{
    if(event->type() == EventType::KeyPressEventType)
    {
        KeyPressEvent *ev = (KeyPressEvent*)event;
        if(ev->key() == Key::KeyEscape && (g_menu_mode == MenuMode::Replay || g_menu_mode == MenuMode::Play
                                          || g_menu_mode == MenuMode::NextLevel))
        {
            if(g_menu_mode == MenuMode::Replay)
                g_replay_stage->setFreeze(false);
            else if(g_menu_mode == MenuMode::Play || g_menu_mode == MenuMode::NextLevel)
                g_world_stage->setFreeze(false);
            setMenu(MenuMode::Hidden);

            return true;
        }
        if(g_menu_mode == MenuMode::Login)
        {
            if(ev->key() == Key::KeyBackspace)
                g_username = g_username.substr(0, g_username.length() - 1);
            else
            {
                char c = key::toChar((Key)ev->key());
                if(c == 0) return true;
                if(ev->modifiers() & Modifier::ShiftModifier)
                    c = (char)toupper(c);
                g_username = g_username + c;
            }
            g_login_entry->setText("Login: " + g_username);
            g_login_btn->setVisible(!g_username.empty());
        }
    }
    return true;
}


AbstractComponent *delegates::componentLoader(unsigned int type, std::ifstream &ifs)
{
    if (type == ComponentType::BarComponentType) return new BarComponent(ifs);
    else if (type == ComponentType::InputComponentType) return new InputComponent(ifs);
    else if (type == ComponentType::MeshComponentType) return new MeshComponent(ifs);
    else if (type == ComponentType::PropertyComponentType) return new PropertyComponent<unsigned int>(ifs);
    else if (type == GameComponentType::NPCComponentType) return new NPCComponent(ifs);
    throw "Unknown type";
}

bool delegates::save_btn_handler()
{
    dump();
    return true;
}

bool delegates::load_btn_handler()
{
    setMenu(MenuMode::Play);
    load();
    return true;
}

bool delegates::exit_btn_handler()
{
    g_window->close();
    return true;
}

void delegates::replay_finished_handler(ReplayStage *)
{
    setMenu(MenuMode::Replay);
}

bool delegates::replay_btn_handler()
{
    g_replay_stage = new ReplayStage(":/trackings/track.dat");
    Object *object = new Object("player");
    object->setUserType(UserObjectType::PlayerType);
    MeshComponent *component = new MeshComponent("mesh", ":/models/bot/bot4.obj");
    object->addComponent(component);
    g_replay_stage->setObject(object);
    g_replay_stage->setInputDelegate(delegates::replayInputDelegate);
    g_replay_stage->setFinishedDelegate(delegates::replay_finished_handler);
    g_replay_stage->setFreeze(true);

    Ground *ground = new Ground(Rect(-((float)BASE_LEVEL_SIZE / 2) * (float)g_level,
                                     -((float)BASE_LEVEL_SIZE / 2) * (float)g_level,
                                     BASE_LEVEL_SIZE * (float)g_level, BASE_LEVEL_SIZE * (float)g_level),
                                ":/models/ground/color.jpg");
    Camera *camera = new Camera();
    g_replay_stage->setGround(ground);
    g_replay_stage->setCamera(camera);
    camera->setTargetObject(object);
    camera->setPosition(Vec3(50.0f, 100.0f, -200.0f));

    g_window->eraseStage(g_world_stage);
    g_window->insertStage(g_replay_stage, 0);
    setMenu(MenuMode::Replay);
    return true;
}

bool delegates::return_btn_handler()
{
    if(g_menu_mode == MenuMode::Replay)
    {
        g_window->insertStage(g_world_stage, 0);
        g_window->disposeStage(g_replay_stage);
        g_replay_stage = nullptr;
        setMenu(MenuMode::Play);
    }
    else if(g_menu_mode == MenuMode::Scoreboard)
    {
        setMenu(g_last_menu_mode.value());
    }

    return true;
}

bool delegates::login_btn_handler()
{
    if(g_username.empty()) return true;
    g_username_widget->setText(g_username);
    setMenu(MenuMode::Play);
    return true;
}

bool delegates::scoreboard_btn_handler()
{
    g_last_menu_mode = g_menu_mode;
    setMenu(MenuMode::Scoreboard);
    return true;
}

bool delegates::close_requested_handler()
{
    ::putStats();
    return true;
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
        g_window = new GameWindow("NGE", 1000, 700, 0);
        g_window->setCloseRequestedDelegate(&delegates::close_requested_handler);
        GraphStage *graph = new GraphStage();
        g_username_widget = new TextWidget("", ":/fonts/arial.ttf", 20, Vec4(1, 1, 1, 1));
        g_level_widget = new TextWidget("Level " + std::to_string(g_level), ":/fonts/arial.ttf", 20, Vec4(1, 1, 1, 1));
        g_points_widget = new TextWidget("Points: " + std::to_string(g_points), ":/fonts/arial.ttf", 20, Vec4(1, 1, 1, 1));
        g_enemies_widget = new TextWidget("Enemies: " + std::to_string(g_enemies), ":/fonts/arial.ttf", 20, Vec4(1, 1, 1, 1));
        g_arenas_widget = new TextWidget("Arenas: " + std::to_string(g_arenas), ":/fonts/arial.ttf", 20, Vec4(1, 1, 1, 1));
        g_username_widget->setPos(Vec2(-130, -40));
        g_level_widget->setPos(Vec2(-130, -80));
        g_points_widget->setPos(Vec2(-130, -120));
        g_enemies_widget->setPos(Vec2(-130, -160));
        g_arenas_widget->setPos(Vec2(-130, -200));
        g_username_widget->setLayoutOrigin(LayoutOrigin::Top | LayoutOrigin::Right);
        g_level_widget->setLayoutOrigin(LayoutOrigin::Top | LayoutOrigin::Right);
        g_points_widget->setLayoutOrigin(LayoutOrigin::Top | LayoutOrigin::Right);
        g_enemies_widget->setLayoutOrigin(LayoutOrigin::Top | LayoutOrigin::Right);
        g_arenas_widget->setLayoutOrigin(LayoutOrigin::Top | LayoutOrigin::Right);
        g_username_widget->setZPos(-0.7);
        g_level_widget->setZPos(-0.7);
        g_points_widget->setZPos(-0.7);
        g_arenas_widget->setZPos(-0.7);
        g_enemies_widget->setZPos(-0.7);
        graph->addWidget(g_username_widget);
        graph->addWidget(g_level_widget);
        graph->addWidget(g_points_widget);
        graph->addWidget(g_enemies_widget);
        graph->addWidget(g_arenas_widget);
        g_background = new RectWidget(Rect(0, 0, 100, 100), Vec4(0, 0, 0, 0.5));
        g_background->setLayoutOrigin(LayoutOrigin::Bottom | LayoutOrigin::Left);
        g_background->setLayoutFlags(LayoutFlags::SizeRelative);
        g_background->setZPos(-0.3);
        graph->addWidget(g_background);

        g_lost_label = new TextWidget("Lost!", ":/fonts/arial.ttf", 75);
        g_lost_label->setZPos(-0.2);
        g_lost_label->setLayoutOrigin(LayoutOrigin::Center);
        g_lost_label->setPos(Vec2(-130, 35));

        g_login_entry = new TextWidget("Login: ", ":/fonts/arial.ttf", 40);
        g_login_entry->setZPos(-0.2);
        g_login_entry->setLayoutOrigin(LayoutOrigin::Center);
        g_login_entry->setPos(Vec2(-130, 35));

        g_next_level_label = new TextWidget("Next level!", ":/fonts/arial.ttf", 75);
        g_next_level_label->setZPos(-0.2);
        g_next_level_label->setLayoutOrigin(LayoutOrigin::Center);
        g_next_level_label->setPos(Vec2(-130, 35));

        g_replay_btn = new ButtonWidget("Replay", Rect(0, 0, 300, 50), &delegates::replay_btn_handler,
                                        Vec4(1, 1, 1, 1), Vec4(0, 0, 0, 1), 40);
        g_replay_btn->setZPos(-0.5);

        g_return_btn = new ButtonWidget("Return", Rect(0, 0, 300, 50), &delegates::return_btn_handler,
                                        Vec4(1, 1, 1, 1), Vec4(0, 0, 0, 1), 40);
        g_return_btn->setZPos(-0.5);

        g_save_btn = new ButtonWidget("Save", Rect(0, 0, 300, 50), &delegates::save_btn_handler,
                                      Vec4(1, 1, 1, 1), Vec4(0, 0, 0, 1), 40);
        g_save_btn->setZPos(-0.5);

        g_load_btn = new ButtonWidget("Load", Rect(0, 0, 300, 50), &delegates::load_btn_handler,
                                      Vec4(1, 1, 1, 1), Vec4(0, 0, 0, 1), 40);
        g_load_btn->setZPos(-0.5);

        g_login_btn = new ButtonWidget("Login", Rect(0, 0, 300, 50), &delegates::login_btn_handler,
                                       Vec4(1, 1, 1, 1), Vec4(0, 0, 0, 1), 40);
        g_login_btn->setZPos(-0.5);

        g_exit_btn = new ButtonWidget("Exit", Rect(0, 0, 300, 50), &delegates::exit_btn_handler,
                                      Vec4(1, 1, 1, 1), Vec4(0, 0, 0, 1), 40);
        g_exit_btn->setZPos(-0.5);

        g_scoreboard_btn = new ButtonWidget("Scoreboard", Rect(0, 0, 300, 50), &delegates::scoreboard_btn_handler,
                                      Vec4(1, 1, 1, 1), Vec4(0, 0, 0, 1), 40);
        g_scoreboard_btn->setZPos(-0.5);

        g_scoreboard = new ScoreboardWidget({ });
        g_scoreboard_btn->setZPos(-0.5);

        graph->addWidget(g_replay_btn);
        graph->addWidget(g_save_btn);
        graph->addWidget(g_load_btn);
        graph->addWidget(g_return_btn);
        graph->addWidget(g_exit_btn);
        graph->addWidget(g_lost_label);
        graph->addWidget(g_login_btn);
        graph->addWidget(g_scoreboard_btn);
        graph->addWidget(g_login_entry);
        graph->addWidget(g_next_level_label);
        graph->addWidget(g_scoreboard);
        g_window->setCursorVisibility(false);
        setLevel(1);
        graph->setFallbackEventHandler(&delegates::graphInputDelegate);
        g_window->pushStage(graph);
        setMenu(MenuMode::Login);

        dump();

        g_window->exec();
        delete g_window;
        Game::shutdown();
    }
    catch (const char *e)
    {
        log() - Fatal < e <<= trace;
    }
}


GComplex createComplex(Vec2 pos, unsigned int id, unsigned int difficulty)
{
    GComplex complex;

    Object *arena = new Object("arena" + std::to_string(id));
    arena->setUserType(UserObjectType::ArenaType);
    MeshComponent *arena_comp = new MeshComponent("mesh", ":/models/arena/arena.fbx");
    arena->addComponent(arena_comp);
    BarComponent *arena_life_bar = new BarComponent("life", difficulty * 100, difficulty * 100);
    arena->addComponent(arena_life_bar);
    arena->setPosition(Vec3(pos.x, 0, pos.y));
    complex.Objects.push_back(arena);

    float a = (float)360 / (float)difficulty;
    for(unsigned int i = 0; i < difficulty; i++)
    {
        Object *enemy = new Object("enemy" + std::to_string(id) + std::to_string(i));
        enemy->setUserType(UserObjectType::EnemyType);
        MeshComponent *enemy_comp = new MeshComponent("mesh", ":/models/bot/bot4.obj");
        NPCComponent *enemy_npc = new NPCComponent("npc");
        BarComponent *enemy_life = new BarComponent("life", difficulty * 30, difficulty * 30);
        float x = (COMPLEX_RADIUS - 50) * glm::acos(glm::radians(a * (float)i));
        float y = (COMPLEX_RADIUS - 50) * glm::asin(glm::radians(a * (float)i));
        enemy->setPosition(Vec3(pos.x + x, 0, pos.y + y));
        enemy->addComponent(enemy_comp);
        enemy->addComponent(enemy_npc);
        enemy->addComponent(enemy_life);
        complex.Objects.push_back(enemy);
    }

    complex.Arenas = 1;
    complex.Enemies = difficulty;

    return complex;
}

int generateRand(int low, int high)
{
    std::random_device dev;
    static std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(low, high);
    return (int)dist(rng);
}

void setLevel(unsigned int level)
{
    if(g_world_stage != nullptr)
    {
        g_window->disposeStage(g_world_stage);
    }

    WorldStage *stage = new WorldStage();
    stage->setCollisionDelegate(&delegates::collisionDelegate);
    stage->setInputDelegate(&delegates::inputDelegate);
    stage->setOffworldDelegate(&delegates::offworldDelegate);

    Ground *ground = new Ground(Rect(-((float)BASE_LEVEL_SIZE / 2) * (float)level,
                                     -((float)BASE_LEVEL_SIZE / 2) * (float)level,
                                     BASE_LEVEL_SIZE * (float)level, BASE_LEVEL_SIZE * (float)level),
                                ":/models/ground/color.jpg");
    stage->setGround(ground);

    unsigned int enemies = 0;
    unsigned int arenas = 0;

    std::vector<Vec2> areas;
    areas.reserve(level);
    //generate complexes
    for(unsigned int i = 0; i < level; i++)
    {
        while(true)
        {
            Vec2 pos = Vec2(
                    generateRand(COMPLEX_RADIUS - (BASE_LEVEL_SIZE / 2) * (int)level, (BASE_LEVEL_SIZE / 2) * (int)level - COMPLEX_RADIUS),
                    generateRand(COMPLEX_RADIUS - (BASE_LEVEL_SIZE / 2) * (int)level, (BASE_LEVEL_SIZE / 2) * (int)level - COMPLEX_RADIUS));
            bool collides = false;
            for (auto &area: areas)
            {
                if (interface::checkCollision(area.x - ((float)COMPLEX_RADIUS / 2), area.x + ((float)COMPLEX_RADIUS / 2),
                                              pos.x - ((float)COMPLEX_RADIUS / 2), pos.x + ((float)COMPLEX_RADIUS / 2))
                    && interface::checkCollision(area.y - ((float)COMPLEX_RADIUS / 2), area.y + ((float)COMPLEX_RADIUS / 2),
                                                 pos.y - ((float)COMPLEX_RADIUS / 2), pos.y + ((float)COMPLEX_RADIUS / 2)))
                {
                    collides = true;
                    break;
                }
            }

            if(collides) continue;

            areas.push_back(pos);
            GComplex complex = createComplex(pos, i, level);
            arenas += complex.Arenas;
            enemies += complex.Enemies;
            for (auto &obj : complex.Objects)
            {
                stage->addObject(obj);
            }
            break;
        }
    }

    Vec2 player_pos;
    while(true)
    {
        player_pos = Vec2(generateRand(50 - ((BASE_LEVEL_SIZE / 2) * (int)level), ((BASE_LEVEL_SIZE / 2) * (int)level) - 50),
                          generateRand(50 - ((BASE_LEVEL_SIZE / 2) * (int)level), ((BASE_LEVEL_SIZE / 2) * (int)level) - 50));

        bool collides = false;
        for (auto &area : areas)
        {
            if(interface::checkCollision(area.x - ((float)COMPLEX_RADIUS / 2), area.x + ((float)COMPLEX_RADIUS / 2),
                                         player_pos.x - 100, player_pos.x + 100)
                && interface::checkCollision(area.y - ((float)COMPLEX_RADIUS / 2), area.y + ((float)COMPLEX_RADIUS / 2),
                                          player_pos.y - 100, player_pos.y + 100))
            {
                collides = true;
                break;
            }
        }

        if(!collides) break;
    }

    Object *player = new Object("player");
    player->setUserType(UserObjectType::PlayerType);
    Camera *camera = new Camera();
    camera->setTargetObject(player);
    camera->setPosition(Vec3(0.0f, 150.0f, -150.0f));
    MeshComponent *player_mesh = new MeshComponent("mesh", ":/models/bot/bot4.obj");
    player->addComponent(player_mesh);
    InputComponent *player_input = new InputComponent("player_input");
    player->addComponent(player_input);
    player->setPosition(Vec3(player_pos.x, 0, player_pos.y));
    stage->addObject(player);
    stage->setTrackFilePath(":/trackings/track.dat");
    stage->setTrackObject("player");
    stage->setCamera(camera);
    stage->setFreeze(true);

    g_world_stage = stage;
    g_window->insertStage(g_world_stage, 0);

    g_level = level;
    g_enemies = enemies;
    g_arenas = arenas;
    g_level_widget->setText("Level " + std::to_string(g_level));
    g_enemies_widget->setText("Enemies: " + std::to_string(g_enemies));
    g_arenas_widget->setText("Arenas: " + std::to_string(g_arenas));
}

void setMenu(MenuMode mode)
{
    if(mode == MenuMode::Play || mode == MenuMode::NextLevel || mode == MenuMode::Lost)
        putStats();

    switch (mode)
    {
        case Hidden:
            g_window->setCursorVisibility(false);
            g_background->setVisible(false);
            g_replay_btn->setVisible(false);
            g_save_btn->setVisible(false);
            g_load_btn->setVisible(false);
            g_exit_btn->setVisible(false);
            g_return_btn->setVisible(false);
            g_login_btn->setVisible(false);
            g_scoreboard_btn->setVisible(false);
            g_lost_label->setVisible(false);
            g_login_entry->setVisible(false);
            g_next_level_label->setVisible(false);
            g_scoreboard->setVisible(false);
            break;
        case Login:
            g_window->setCursorVisibility(true);
            g_login_entry->setPos(Vec2(-150, 40));
            g_login_btn->setPos(Vec2(-150, -25));
            g_exit_btn->setPos(Vec2(-150, -80));

            g_background->setVisible(true);
            g_replay_btn->setVisible(false);
            g_save_btn->setVisible(false);
            g_load_btn->setVisible(false);
            g_exit_btn->setVisible(true);
            g_return_btn->setVisible(false);
            g_login_btn->setVisible(!g_username.empty());
            g_scoreboard_btn->setVisible(false);
            g_lost_label->setVisible(false);
            g_login_entry->setVisible(true);
            g_next_level_label->setVisible(false);
            g_scoreboard->setVisible(false);
            break;
        case Play:
            g_window->setCursorVisibility(true);
            g_scoreboard_btn->setPos(Vec2(-150, 90));
            g_replay_btn->setPos(Vec2(-150, 35));
            g_save_btn->setPos(Vec2(-150, -20));
            g_load_btn->setPos(Vec2(-150, -75));
            g_exit_btn->setPos(Vec2(-150, -130));

            g_background->setVisible(true);
            g_replay_btn->setVisible(true);
            g_save_btn->setVisible(true);
            g_load_btn->setVisible(true);
            g_exit_btn->setVisible(true);
            g_return_btn->setVisible(false);
            g_login_btn->setVisible(false);
            g_scoreboard_btn->setVisible(true);
            g_lost_label->setVisible(false);
            g_login_entry->setVisible(false);
            g_next_level_label->setVisible(false);
            g_scoreboard->setVisible(false);
            break;
        case Replay:
            g_window->setCursorVisibility(true);
            g_replay_btn->setPos(Vec2(-150, 30));
            g_return_btn->setPos(Vec2(-150, -25));
            g_exit_btn->setPos(Vec2(-150, -80));

            g_background->setVisible(true);
            g_replay_btn->setVisible(true);
            g_save_btn->setVisible(false);
            g_load_btn->setVisible(false);
            g_exit_btn->setVisible(true);
            g_return_btn->setVisible(true);
            g_login_btn->setVisible(false);
            g_scoreboard_btn->setVisible(false);
            g_lost_label->setVisible(false);
            g_login_entry->setVisible(false);
            g_next_level_label->setVisible(false);
            g_scoreboard->setVisible(false);
            break;
        case Lost:
            g_window->setCursorVisibility(true);
            g_lost_label->setPos(Vec2(-150, 72));
            g_scoreboard_btn->setPos(Vec2(-150, 13));
            g_replay_btn->setPos(Vec2(-150, -42));
            g_load_btn->setPos(Vec2(-150, -97));
            g_exit_btn->setPos(Vec2(-150, -152));

            g_background->setVisible(true);
            g_replay_btn->setVisible(true);
            g_save_btn->setVisible(false);
            g_load_btn->setVisible(true);
            g_exit_btn->setVisible(true);
            g_return_btn->setVisible(false);
            g_login_btn->setVisible(false);
            g_scoreboard_btn->setVisible(true);
            g_lost_label->setVisible(true);
            g_login_entry->setVisible(false);
            g_next_level_label->setVisible(false);
            g_scoreboard->setVisible(false);
            break;
        case NextLevel:
            g_window->setCursorVisibility(true);
            g_next_level_label->setPos(Vec2(-150, 160));
            g_scoreboard_btn->setPos(Vec2(-150, 90));
            g_replay_btn->setPos(Vec2(-150, 35));
            g_save_btn->setPos(Vec2(-150, -20));
            g_load_btn->setPos(Vec2(-150, -75));
            g_exit_btn->setPos(Vec2(-150, -130));

            g_background->setVisible(true);
            g_replay_btn->setVisible(true);
            g_save_btn->setVisible(true);
            g_load_btn->setVisible(true);
            g_exit_btn->setVisible(true);
            g_return_btn->setVisible(false);
            g_login_btn->setVisible(false);
            g_scoreboard_btn->setVisible(true);
            g_lost_label->setVisible(false);
            g_login_entry->setVisible(false);
            g_next_level_label->setVisible(true);
            g_scoreboard->setVisible(false);
            break;
        case Scoreboard:
            g_window->setCursorVisibility(true);
            g_return_btn->setPos(Vec2(-150, -300));

            std::vector<StatsEntry> entries = StatsManager::getTopFive(":/stats/stats.dat");
            g_scoreboard->setEntries(entries);

            g_background->setVisible(true);
            g_replay_btn->setVisible(false);
            g_save_btn->setVisible(false);
            g_load_btn->setVisible(false);
            g_exit_btn->setVisible(false);
            g_return_btn->setVisible(true);
            g_login_btn->setVisible(false);
            g_scoreboard_btn->setVisible(false);
            g_lost_label->setVisible(false);
            g_login_entry->setVisible(false);
            g_next_level_label->setVisible(false);
            g_scoreboard->setVisible(true);
            break;
    }
    g_menu_mode = mode;
}

void load()
{
    try
    {
        GSaveData data;
        WorldStage *world_stage_t = SaveManager::load(":/save/save.dat", &data, sizeof(GSaveData));
        world_stage_t->setCollisionDelegate(&delegates::collisionDelegate);
        world_stage_t->setInputDelegate(&delegates::inputDelegate);
        world_stage_t->setOffworldDelegate(&delegates::offworldDelegate);
        Object *player = world_stage_t->findObjectByName("player");
        world_stage_t->camera()->setTargetObject(player);
        world_stage_t->setFreeze(true);
        g_window->disposeStage(g_world_stage);
        g_world_stage = world_stage_t;
        g_window->insertStage(g_world_stage, 0);
        g_enemies = data.Enemies;
        g_arenas = data.Arenas;
        g_level = data.Level;
        g_points = data.Points;

        g_level_widget->setText("Level " + std::to_string(g_level));
        g_enemies_widget->setText("Enemies: " + std::to_string(g_enemies));
        g_arenas_widget->setText("Arenas: " + std::to_string(g_arenas));
        g_points_widget->setText("Points: " + std::to_string(g_points));
    }
    catch (const char *err)
    {
        log() - Critical < "Failed to load save:" < err;
    }
}

void dump()
{
    try
    {
        GSaveData data { g_enemies, g_arenas, g_level, g_points };
        SaveManager::dump(g_world_stage, ":/save/save.dat", &data, sizeof(GSaveData));
    }
    catch (char *err)
    {
        log() - Critical < "Failed to save:" < err;
    }
}

void putStats()
{
    StatsEntry entry;
    strcpy(entry.Player, g_username.c_str());
    entry.Level = g_level;
    entry.Points = g_points;
    StatsManager::put(":/stats/stats.dat", entry);
}