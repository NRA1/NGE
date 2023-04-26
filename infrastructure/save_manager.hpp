#ifndef NGE_SAVE_MANAGER_HPP
#define NGE_SAVE_MANAGER_HPP


#include "../interface/world_stage.hpp"

class SaveManager
{
public:
    static WorldStage *load(const std::string &path, void *userdata = nullptr, unsigned int userdata_size = 0);
    static void dump(WorldStage *world_stage, const std::string &path, void *userdata = nullptr, unsigned int userdata_size = 0);

    static void setComponentLoader(AbstractComponent *(*componentFactory)(unsigned int, std::ifstream&));

private:
    static std::vector<Object *> load_objects(std::ifstream &ifs);
    static std::vector<AbstractComponent*> load_components(std::ifstream &ifs);
    static Camera *load_camera(std::ifstream &ifs);
    static Ground *load_ground(std::ifstream &ifs);
    static Rotation load_rotation(std::ifstream &ifs);
    static Rect load_rect(std::ifstream &ifs);

    static void dump_objects(std::vector<Object*> &objects, std::ofstream &ofs);
    static void dump_components(std::vector<AbstractComponent*> &components, std::ofstream &ofs);
    static void dump_camera(Camera &camera, std::ofstream &ofs);
    static void dump_ground(Ground &ground, std::ofstream &ofs);
    static void dump_rotation(Rotation &rotation, std::ofstream &ofs);
    static void dump_rect(Rect &rect, std::ofstream &ofs);

    static AbstractComponent *(*component_factory_)(unsigned int type, std::ifstream &ifs);
};


#endif //NGE_SAVE_MANAGER_HPP
