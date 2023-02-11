#include "world_stage.hpp"
#include "../infrastructure/logger.hpp"

WorldStage::WorldStage() : viewport_(Size(-1, -1)), camera_(nullptr), shader_program_(nullptr) { }

void WorldStage::viewportSizeChanged(Size size)
{
    viewport_ = size;
    glViewport(0, 0, viewport_.width(), viewport_.height());
}

void WorldStage::render()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader_program_->use();

    if(camera_ == nullptr)
    {
        log() - Critical < "No camera set";
        return;
    }

    camera_->setStageViewport();
    for (Object *object : objects_)
    {
        object->render();
    }
}

void WorldStage::addObject(Object *object)
{
    objects_.push_back(object);
    object->setStage(this);
}

void WorldStage::removeObject(Object *object)
{
    for (long unsigned int i = 0; i < objects_.size(); ++i)
    {
        if(objects_[i] == object)
        {
            objects_.erase(objects_.begin() + i);
            objects_[i]->setStage(nullptr);
            delete objects_[i];
        }
    }
}

void WorldStage::onAppearing()
{
    shader_program_ = new ShaderProgram(":/shaders/vertex.vert", ":/shaders/fragment.frag");
    for (Object *object : objects_)
    {
        object->load();
    }
}


void WorldStage::onDisappearing()
{
    for(Object *object : objects_)
    {
        object->unload();
    }
}

void WorldStage::setCamera(Camera *camera)
{
    camera_ = camera;
    camera_->setShaderProgram(&shader_program_);
    camera_->setViewport(&viewport_);
}

ShaderProgram *WorldStage::shaderProgram()
{
    return shader_program_;
}

void WorldStage::handleEvent(Event *event)
{
    if(event->type() == KeyPressEventType)
    {
        KeyPressEvent *ev = (KeyPressEvent*)event;
        if(ev->key() == KeyW)
        {
            objects_[0]->setPosition(objects_[0]->position() + Vec3(0.1f, 0.0f, 0.0f));
        }
    }
}

WorldStage::~WorldStage()
{
    for (Object *object : objects_)
    {
        delete object;
    }
    delete camera_;
    delete shader_program_;
}


