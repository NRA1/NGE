#include "mesh_component.hpp"
#include "../infrastructure/save_manager.hpp"

#include <utility>

MeshComponent::MeshComponent(std::string name, std::string model_path)
        : AbstractMeshComponent(std::move(name)), model_(nullptr), model_path_(std::move(model_path)), loaded_(false)
{

}

const PositionedBox &MeshComponent::boundingBox() const
{
    if(model_ == nullptr)
    {
        log() - Critical < "Model not set, returning invalid bounding box.";
        log() - Critical < "Memory leak!";
        return *new PositionedBox(-1, -1, -1);
    }
    return model_->boundingBox();
}

void MeshComponent::load()
{
    model_ = new Model(model_path_);

    setModelPosition();

    loaded_ = true;
}

void MeshComponent::unload()
{
    delete model_;
    model_ = nullptr;
    loaded_ = false;
}

void MeshComponent::render()
{
    setModelPosition();
    model_->render(*object()->stage()->shaderProgram());
}

MeshComponent::~MeshComponent()
{
    delete model_;
}

void MeshComponent::objectChanged()
{
}

void MeshComponent::setModelPosition()
{
    if(!loaded_ && object() != nullptr) return;
    Mat4 model = Mat4(1.0f);
    model = glm::translate(model, object()->position());

    std::optional<Rotation> rot = object()->motionVector().grabbedRotation();
    Rotation physics_rot;
    if(rot.has_value()) physics_rot = object()->rotation() + rot.value();
    else physics_rot = object()->rotation();

    model = glm::rotate(model, glm::radians(physics_rot.roll()), Vec3(0, 1, 0));
//    model = glm::rotate(model, glm::radians(physics_rot.pitch()), Vec3(0, 0, 1));
//    model = glm::rotate(model, glm::radians(physics_rot.yaw()), Vec3(1, 0, 0));
    object()->stage()->shaderProgram()->setMat4("model", model);
}

unsigned int MeshComponent::type()
{
    return MeshComponentType;
}

void MeshComponent::dump(std::ofstream &ofs)
{
    AbstractMeshComponent::dump(ofs);
    if(!ofs.is_open()) throw "File not open";
    char path[50];
    for(unsigned int i = 0; i < model_path_.size(); i++)
        path[i] = model_path_[i];
    path[model_path_.size()] = '\0';
    ofs.write(path, sizeof(path));
}

MeshComponent::MeshComponent(std::ifstream &ifs) : AbstractMeshComponent(ifs), loaded_(false)
{
    char path[50];
    ifs.read(path, sizeof(path));
    model_path_ = std::string(path);
}
