#include "abstract_mesh_component.hpp"

unsigned int AbstractMeshComponent::type()
{
    return ComponentType::AbstractMeshComponentType;
}

AbstractMeshComponent::AbstractMeshComponent(std::ifstream &ifs) : AbstractComponent(ifs)
{

}
