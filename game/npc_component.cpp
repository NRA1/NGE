#include "npc_component.hpp"

#include <utility>

void NPCComponent::tick()
{
    Object *player = object()->stage()->findObjectByName("player");
    if(player == nullptr) return;
    Vec3 vec = object()->position();
    vec = player->position() - vec;
    float len = glm::length(vec);
    float z = vec.z < 0 ? -vec.z : vec.z;
    float rot = glm::degrees(glm::asin(z / len));
    if(vec.z < 0 && vec.x > 0) rot = 360 - rot;
    else if(vec.z < 0 && vec.x < 0) rot = 180 + rot;
    else if(vec.z > 0 && vec.x < 0) rot = 180 - rot;
    Rotation r = object()->rotation();
    r.setRoll(-rot + 90);
    object()->setRotation(r);
    mmo_->setVelocity(Vec3(0, 0, 7));
}

void NPCComponent::objectChanged()
{
    delete mmo_;
    mmo_ = object()->motionVector().generateManipulationObject(false);
}

NPCComponent::NPCComponent(std::string name) : AbstractNPCComponent(std::move(name)), mmo_(nullptr)
{

}
