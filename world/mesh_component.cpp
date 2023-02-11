#include "mesh_component.hpp"

#include <utility>

MeshComponent::MeshComponent(std::string name, std::string model_path)
        : AbstractMeshComponent(std::move(name)), model_(nullptr), model_path_(std::move(model_path)), loaded_(false)
{

}

const Box &MeshComponent::boundingBox() const
{
    if(model_ == nullptr)
    {
        log() - Critical < "Model not set, returning invalid bounding box.";
        log() - Critical < "Memory leak!";
        return *new Box(-1, -1, -1);
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
    model = glm::scale(model, Vec3(1.0f, 1.0f, 1.0f));
    object()->stage()->shaderProgram()->setMat4("model", model);
}

int MeshComponent::type()
{
    return MeshComponentType;
}
