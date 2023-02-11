#ifndef NGE_MESH_COMPONENT_HPP
#define NGE_MESH_COMPONENT_HPP

#include <utility>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/gl.h>
#include "abstract_component.hpp"
#include "../enums/capability.hpp"
#include "../infrastructure/resource_loader.hpp"
#include "../infrastructure/common_fallbacks.hpp"
#include "../infrastructure/game.hpp"
#include "../data_objects/shader_program.hpp"
#include "../data_objects/model.hpp"
#include "../interface/world_stage.hpp"

class MeshComponent : public AbstractComponent
{
public:
    explicit MeshComponent(std::string name, std::string model_path);
    int capabilities() override;
    [[nodiscard]] const Box &boundingBox() const override;
    void load() override;
    void unload() override;
    void render() override;
    void objectChanged() override;
    void setModelPosition() override;

    ~MeshComponent() override;

private:
    Model *model_;
    Box bounding_box_;

    std::string model_path_;

    bool loaded_;
};


#endif //NGE_MESH_COMPONENT_HPP
