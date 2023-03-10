#ifndef NGE_NPC_COMPONENT_HPP
#define NGE_NPC_COMPONENT_HPP

#include "../world/object.hpp"
#include "../interface/world_stage.hpp"
#include "../world/abstractions/abstract_npc_component.hpp"

class NPCComponent : public AbstractNPCComponent
{
public:
    NPCComponent(std::string name);

    void tick() override;
    void objectChanged() override;

private:
    MotionManipulationObject *mmo_;
};


#endif //NGE_NPC_COMPONENT_HPP