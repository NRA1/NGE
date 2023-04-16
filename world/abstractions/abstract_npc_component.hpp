#ifndef NGE_ABSTRACT_NPC_COMPONENT_HPP
#define NGE_ABSTRACT_NPC_COMPONENT_HPP


#include "abstract_component.hpp"

class AbstractNPCComponent : public AbstractComponent
{
public:
    AbstractNPCComponent(std::string name);
    AbstractNPCComponent(std::ifstream &ifs);
    unsigned int type() override;
    virtual void tick() = 0;
};


#endif //NGE_ABSTRACT_NPC_COMPONENT_HPP
