#ifndef NGE_MESH_COMPONENT_HPP
#define NGE_MESH_COMPONENT_HPP

#include <utility>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/gl.h>
#include "../enums/component_type.hpp"
#include "../infrastructure/resource_loader.hpp"
#include "../infrastructure/common_fallbacks.hpp"
#include "../infrastructure/game.hpp"
#include "../data_objects/shader_program.hpp"
#include "../data_objects/model.hpp"
#include "../interface/world_stage.hpp"
#include "abstractions/abstract_mesh_component.hpp"

class MeshComponent : public AbstractMeshComponent
{
public:
    explicit MeshComponent(std::string name, std::string model_path);
    [[nodiscard]] const Box &boundingBox() const override;
    int type() override;
    void load() override;
    void unload() override;
    void render() override;
    void objectChanged() override;
    void setModelPosition() override;

    ~MeshComponent() override;

private:
    Model *model_;

    std::string model_path_;

    bool loaded_;
};


#endif //NGE_MESH_COMPONENT_HPP
