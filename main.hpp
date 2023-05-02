#ifndef NGE_MAIN_HPP
#define NGE_MAIN_HPP

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
#include "game/scoreboard_widget.hpp"
#include "enums/modifier.hpp"
#include <random>


#define COMPLEX_RADIUS 400
#define BASE_LEVEL_SIZE 4000

struct GComplex
{
    std::vector<Object*> Objects;
    unsigned int Arenas;
    unsigned int Enemies;
};

struct GSaveData
{
    unsigned int Enemies;
    unsigned int Arenas;
    unsigned int Level;
    unsigned int Points;
};

enum MenuMode
{
    Hidden,
    Login,
    Play,
    Replay,
    Lost,
    NextLevel,
    Scoreboard
} g_menu_mode;

void run();
GComplex createComplex(Vec2 pos, unsigned int id, unsigned int difficulty);
int generateRand(int low, int high);
void setLevel(unsigned int level);
void setMenu(MenuMode mode);
void load();
void dump();
void putStats();


namespace delegates
{
    unsigned int bullet_counter = 0;

    bool collisionDelegate(WorldStage *stage, Object *object, Object *collider);
    bool inputDelegate(WorldStage *stage, Event *event);
    bool offworldDelegate(WorldStage *stage, Object *obj);
    bool replayInputDelegate(ReplayStage *stage, Event *event);
    bool graphInputDelegate(GraphStage *stage, Event *event);
    AbstractComponent *componentLoader(unsigned int type, std::ifstream &ifs);
    bool save_btn_handler();
    bool load_btn_handler();
    bool exit_btn_handler();
    void replay_finished_handler(ReplayStage *);
    bool replay_btn_handler();
    bool return_btn_handler();
    bool login_btn_handler();
    bool scoreboard_btn_handler();
    bool close_requested_handler();
}

unsigned int g_enemies = 0;
unsigned int g_arenas = 0;
unsigned int g_level = 0;
unsigned int g_points = 0;

std::string g_username;
std::optional<MenuMode> g_last_menu_mode;

GameWindow *g_window;
WorldStage *g_world_stage = nullptr;
ReplayStage *g_replay_stage = nullptr;

TextWidget *g_username_widget;
TextWidget *g_level_widget;
TextWidget *g_points_widget;
TextWidget *g_enemies_widget;
TextWidget *g_arenas_widget;

RectWidget *g_background;
ButtonWidget *g_replay_btn;
ButtonWidget *g_save_btn;
ButtonWidget *g_load_btn;
ButtonWidget *g_exit_btn;
ButtonWidget *g_return_btn;
ButtonWidget *g_login_btn;
ButtonWidget *g_scoreboard_btn;
TextWidget *g_lost_label;
TextWidget *g_login_entry;
TextWidget *g_next_level_label;
ScoreboardWidget *g_scoreboard;



#endif //NGE_MAIN_HPP
