#include "replay_stage.hpp"
#include "../data_objects/track_stamp.hpp"

#include <utility>

ReplayStage::ReplayStage(std::string track_path) : object_(nullptr), track_file_path_(std::move(track_path)),
                                                   last_stamp_(std::nullopt), input_delegate_(nullptr),
                                                   finished_delegate_(nullptr)
{

}

bool ReplayStage::handleEvent(Event *event)
{
    if(event->type() == KeyPressEventType || event->type() == KeyReleaseEventType || event->type() == MouseMoveEventType
       || event->type() == MousePressEventType || event->type() == MouseReleaseEventType)
    {
        if(input_delegate_ == nullptr) log() - Debug < "No input delegate set, ignoring input event";
        else if(input_delegate_(this, event)) return true;
    }
    return false;
}

void ReplayStage::render(unsigned int time)
{
    if(!freeze())
        tick(time);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader_program_->use();

    if(camera_ == nullptr)
    {
        log() - Critical < "No camera set";
        return;
    }

    camera_->setStageViewport(shader_program_);

    if(ground_ == nullptr)
    {
        log() - Critical < "Ground not set";
    }
    else ground_->render();

    if(object_ != nullptr)
        object_->render();
}

void ReplayStage::onAppearing()
{
    shader_program_ = new ShaderProgram(":/shaders/world/vertex.vert", ":/shaders/world/fragment.frag");
    if(object_ != nullptr)
        object_->load();
    track_file_ = std::ifstream(ResourceLoader::fullPath(track_file_path_), std::ios::binary);
    if(!track_file_.is_open())
    {
        log() - Critical < "Failed to open track file. Unable to replay";
    }
    visible_ = true;
}

void ReplayStage::onDisappearing()
{
    visible_ = false;
    track_file_.close();
    if(object_ != nullptr)
        object_->unload();
    delete shader_program_;
    shader_program_ = nullptr;
}

ReplayStage::~ReplayStage()
{
    delete object_;
}

void ReplayStage::setObject(Object *object)
{
    if(visible_ && object_ != nullptr)
        object_->unload();
    object_ = object;
    object->setStage(this);
    if(visible_ && object_ != nullptr)
        object_->load();
}

void ReplayStage::setInputDelegate(bool (*inputDelegate)(ReplayStage *, Event *))
{
    input_delegate_ = inputDelegate;
}

void ReplayStage::tick(unsigned int time)
{
    if(object_ == nullptr || !track_file_.is_open()) return;
    TrackStamp stamp;
    if(last_stamp_ == std::nullopt)
    {
        if(!track_file_.read((char*)&stamp, sizeof(TrackStamp)))
        {
            if(finished_delegate_ != nullptr) finished_delegate_(this);
            setFreeze(true);
            return;
        }
        object_->setPosition(stamp.Pos);
        object_->setRotation(Rotation(stamp.Rotation));
        stamp.Delta = 0;
        last_stamp_ = stamp;
    }
    else
    {
        if(last_stamp_->Delta + (int)time <= 0)
        {
            float multiplier = (float)time / -(float)last_stamp_->Delta;
            Vec3 dpos = (last_stamp_->Pos - object_->position()) * multiplier;
            Vec3 drot = (last_stamp_->Rotation - object_->rotation().vec3()) * multiplier;
            object_->setPosition(object_->position() + dpos);
            object_->setRotation(object_->rotation().vec3() + drot);
            last_stamp_->Delta = last_stamp_->Delta + time;
        }
        else
        {
            if(!track_file_.read((char*)&stamp, sizeof(TrackStamp)))
            {
                if(finished_delegate_ != nullptr) finished_delegate_(this);
                setFreeze(true);
                return;
            }
            float multiplier = (float)(time + last_stamp_->Delta) / (float)stamp.Delta;
            Vec3 dpos = (stamp.Pos - last_stamp_->Pos) * multiplier;
            Vec3 drot = (stamp.Rotation - last_stamp_->Rotation) * multiplier;
            object_->setPosition(last_stamp_->Pos + dpos);
            object_->setRotation(last_stamp_->Rotation + drot);
            stamp.Delta = -(stamp.Delta - ((int)time - last_stamp_->Delta));
            last_stamp_ = stamp;
        }
    }
}

void ReplayStage::setFinishedDelegate(void (*finishedDelegate)(ReplayStage *))
{
    finished_delegate_ = finishedDelegate;
}