#include "save_manager.hpp"

AbstractComponent *(*SaveManager::component_factory_)(unsigned int, std::ifstream&) = nullptr;

WorldStage *SaveManager::load(const std::string &path)
{
    std::ifstream ifs(ResourceLoader::fullPath(path), std::ios::binary);
    if(!ifs.is_open()) throw "File not open";

    WorldStage *stage = new WorldStage();
    std::vector<Object*> objects = load_objects(ifs);
    for(auto &obj : objects)
        stage->addObject(obj);
    stage->setCamera(load_camera(ifs));
    stage->setGround(load_ground(ifs));
    ifs.close();

    return stage;
}

void SaveManager::dump(WorldStage *world_stage, const std::string &path)
{
    std::ofstream ofs(ResourceLoader::fullPath(path), std::ios::binary);
    if(!ofs.is_open()) throw "File not open";
    dump_objects(world_stage->objects_, ofs);
    dump_camera(*world_stage->camera_, ofs);
    dump_ground(*world_stage->ground_, ofs);
    ofs.close();
}

void SaveManager::dump_objects(std::vector<Object *> &objects, std::ofstream &ofs)
{
    if(!ofs.is_open()) throw "File not open";
    unsigned int len = objects.size();
    ofs.write((char*) &len, sizeof(unsigned int));
    for (auto &obj : objects)
    {
        char name[20];
        for(unsigned int i = 0; i < obj->name_.size(); i++)
            name[i] = obj->name_[i];
        name[obj->name_.size()] = '\0';
        ofs.write((char*)&name, sizeof(name));

        dump_components(obj->components_, ofs);

        ofs.write((char*)&obj->user_types_, sizeof(obj->user_types_));
        ofs.write((char*)&obj->position_, sizeof(obj->position_));
        dump_rotation(obj->rotation_, ofs);
    }
}

void SaveManager::dump_rotation(Rotation &rotation, std::ofstream &ofs)
{
    if(!ofs.is_open()) throw "File not open";
    ofs.write((char*)&rotation.roll_, sizeof(rotation.roll_));
    ofs.write((char*)&rotation.pitch_, sizeof(rotation.pitch_));
    ofs.write((char*)&rotation.yaw_, sizeof(rotation.yaw_));
}

void SaveManager::dump_components(std::vector<AbstractComponent *> &components, std::ofstream &ofs)
{
    if(!ofs.is_open()) throw "File not open";
    unsigned int len = components.size();
    ofs.write((char*) &len, sizeof(unsigned int));
    for(auto &component : components)
    {
        unsigned int type = component->type();
        ofs.write((char*)&type, sizeof(type));
        component->dump(ofs);
    }
}

void SaveManager::dump_camera(Camera &camera, std::ofstream &ofs)
{
    if(!ofs.is_open()) throw "File not open";
    ofs.write((char*)&camera.position_, sizeof(camera.position_));
    dump_rotation(camera.rotation_, ofs);
}

void SaveManager::dump_ground(Ground &ground, std::ofstream &ofs)
{
    if (!ofs.is_open()) throw "File not open";
    dump_rect(ground.bounds_, ofs);
    bool write_sprite = ground.sprite_.has_value();
    ofs.write((char *) &write_sprite, sizeof(write_sprite));
    if (write_sprite)
    {
        char path[50];
        for (unsigned int i = 0; i < ground.sprite_->path_.size(); i++)
            path[i] = ground.sprite_->path_[i];
        path[ground.sprite_->path_.size()] = '\0';
        ofs.write((char*) &path, sizeof(path));
    }
}

void SaveManager::dump_rect(Rect &rect, std::ofstream &ofs)
{
    if(!ofs.is_open()) throw "File not open";
    ofs.write((char*)&rect.x_, sizeof(rect.x_));
    ofs.write((char*)&rect.y_, sizeof(rect.y_));
    ofs.write((char*)&rect.width_, sizeof(rect.width_));
    ofs.write((char*)&rect.height_, sizeof(rect.height_));
}

std::vector<Object *> SaveManager::load_objects(std::ifstream &ifs)
{
    if(!ifs.is_open()) throw "File not open";
    unsigned int len;
    ifs.read((char*) &len, sizeof(unsigned int));
    std::vector<Object*> objects;
    objects.reserve(len);
    for (unsigned int i = 0; i < len; i++)
    {
        char name[20];
        ifs.read((char*)&name, sizeof(name));
        Object *obj = new Object(std::string(name));
        std::vector<AbstractComponent*> components = load_components(ifs);
        for (auto &component : components)
        {
            obj->addComponent(component);
        }


        ifs.read((char*)&obj->user_types_, sizeof(obj->user_types_));
        ifs.read((char*)&obj->position_, sizeof(obj->position_));
        obj->rotation_ = load_rotation(ifs);
        obj->name_ = std::string(name);
        objects.push_back(obj);
    }

    return objects;
}

std::vector<AbstractComponent *> SaveManager::load_components(std::ifstream &ifs)
{
    if(!ifs.is_open()) throw "File not open";
    unsigned int len;
    ifs.read((char*) &len, sizeof(unsigned int));
    std::vector<AbstractComponent*> components;
    components.reserve(len);
    for(unsigned int i = 0; i < len; i++)
    {
        unsigned int type;
        ifs.read((char*)&type, sizeof(type));
        if(component_factory_ == nullptr) throw "Component factory not set";
        AbstractComponent *component = component_factory_(type, ifs);
        components.push_back(component);
    }
    return components;
}

Camera *SaveManager::load_camera(std::ifstream &ifs)
{
    if(!ifs.is_open()) throw "File not open";
    Camera *camera = new Camera();
    ifs.read((char*)&camera->position_, sizeof(camera->position_));
    camera->rotation_ = load_rotation(ifs);
    return camera;
}

Ground *SaveManager::load_ground(std::ifstream &ifs)
{
    if (!ifs.is_open()) throw "File not open";
    Rect bounds = load_rect(ifs);
    bool read_sprite;
    ifs.read((char*)&read_sprite, sizeof(read_sprite));
    std::string diffuse;
    if (read_sprite)
    {
        char path[50];
        ifs.read((char*) &path, sizeof(path));
        diffuse = std::string(path);
    }
    return new Ground(bounds, diffuse);
}

Rotation SaveManager::load_rotation(std::ifstream &ifs)
{
    if(!ifs.is_open()) throw "File not open";
    Rotation rotation;
    ifs.read((char*)&rotation.roll_, sizeof(rotation.roll_));
    ifs.read((char*)&rotation.pitch_, sizeof(rotation.pitch_));
    ifs.read((char*)&rotation.yaw_, sizeof(rotation.yaw_));
    return rotation;
}

Rect SaveManager::load_rect(std::ifstream &ifs)
{
    if(!ifs.is_open()) throw "File not open";
    Rect rect;
    ifs.read((char*)&rect.x_, sizeof(rect.x_));
    ifs.read((char*)&rect.y_, sizeof(rect.y_));
    ifs.read((char*)&rect.width_, sizeof(rect.width_));
    ifs.read((char*)&rect.height_, sizeof(rect.height_));
    return rect;
}

void SaveManager::setComponentLoader(AbstractComponent *(*componentFactory)(unsigned int, std::ifstream&))
{
    component_factory_ = componentFactory;
}

