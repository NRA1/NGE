#ifndef NGE_GRAPH_STAGE_HPP
#define NGE_GRAPH_STAGE_HPP


#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "abstract_stage.hpp"
#include "../data_objects/shader_program.hpp"
#include "../graph/widget.hpp"

class GraphStage : public AbstractStage
{
public:
    GraphStage();

    void onAppearing() override;
    void onDisappearing() override;

    void viewportSizeChanged(Size size) override;
    bool handleEvent(Event *event) override;
    void render(unsigned int time) override;

    void addWidget(Widget *widget);
    void deleteWidget(Widget *widget);

private:
    std::list<Widget *> findUnderlyingWidgets(Vec2 pos) const;
    std::list<Widget *> maskList(const std::list<Widget*>& source, const std::list<Widget*>& mask) const;
    std::map<unsigned int, ShaderProgram *> gatherShaderPrograms(unsigned int types);

    Size viewport_;
    std::map<unsigned int, ShaderProgram *> shader_programs_;
    std::vector<Widget*> widgets_;
    bool shown_;

    std::optional<Vec2> last_mouse_pos_;
    std::list<Widget*> entered_widgets_;
};


#endif //NGE_GRAPH_STAGE_HPP
