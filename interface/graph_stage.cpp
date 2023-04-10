#include "graph_stage.hpp"
#include "interface_helpers.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "../enums/shader_program_type.hpp"

GraphStage::GraphStage() : viewport_(Size(-1, -1)), shown_(false), last_mouse_pos_(std::nullopt)
{

}

void GraphStage::onAppearing()
{
    shader_programs_[ShaderProgramType::ShapeType] = new ShaderProgram(":/shaders/graph/vertex.vert", ":/shaders/graph/fragment.frag");
    shader_programs_[ShaderProgramType::TextType] = new ShaderProgram(":/shaders/text/vertex.vert", ":/shaders/text/fragment.frag");
    glm::mat4 projection = glm::ortho(-(viewport_.width() / 2), viewport_.width() / 2, -(viewport_.height() / 2),
                                      viewport_.height() / 2);
    for(auto &sp : shader_programs_)
        sp.second->setMat4("projection", projection);

    for(auto &widget : widgets_)
        widget->load();
    shown_ = true;
}

void GraphStage::onDisappearing()
{
    for(auto &widget : widgets_)
        widget->unload();
    for(auto &sp : shader_programs_)
        delete sp.second;
    shader_programs_.clear();
    shown_ = false;
}

void GraphStage::viewportSizeChanged(Size size)
{
    viewport_ = size;
    glViewport(0, 0, (int)viewport_.width(), (int)viewport_.height());

    glm::mat4 projection = glm::ortho(-(viewport_.width() / 2), viewport_.width() / 2, -(viewport_.height() / 2),
                                      viewport_.height() / 2);
    for(auto &sp : shader_programs_)
        sp.second->setMat4("projection", projection);
}

bool GraphStage::handleEvent(Event *event)
{
    if(event->type() & EventType::KeyPressEventType && last_mouse_pos_.has_value())
    {
        KeyPressEvent *ev = (KeyPressEvent*)event;
        std::list<Widget*> underlying = findUnderlyingWidgets(last_mouse_pos_.value());
        if(underlying.empty()) return false;
        for(auto &widget : underlying)
            if(widget->keyPressEvent(ev)) break;
    }
    if(event->type() & EventType::KeyReleaseEventType && last_mouse_pos_.has_value())
    {
        KeyReleaseEvent *ev = (KeyReleaseEvent*)event;
        std::list<Widget*> underlying = findUnderlyingWidgets(last_mouse_pos_.value());
        for(auto &widget : underlying)
            if(widget->keyReleaseEvent(ev)) break;
    }
    if(event->type() & EventType::MouseMoveEventType)
    {
        MouseMoveEvent *ev = (MouseMoveEvent*)event;
        Vec2 new_pos = Vec2(ev->x(), ev->y());
        std::list<Widget*> underlying = findUnderlyingWidgets(new_pos);
        std::list<Widget*> new_entered = maskList(underlying, entered_widgets_);
        std::list<Widget*> new_exited = maskList(entered_widgets_, underlying);
        for(auto &widget : new_entered)
            widget->mouseEnterEvent();
        for(auto &widget : new_exited)
            widget->mouseLeaveEvent();
        last_mouse_pos_ = new_pos;
        entered_widgets_ = underlying;
        for(auto &widget : underlying)
            if(widget->mouseMoveEvent(ev)) return true;
    }

    return false;
}

void GraphStage::render(unsigned int)
{
    for(auto &widget : widgets_)
    {
        std::map<unsigned int, ShaderProgram *> shader_programs = gatherShaderPrograms(widget->requiredShaderPrograms());
        Mat4 mat = Mat4(1.0f);
        mat = glm::translate(mat, Vec3(widget->pos(), 0));
        for(auto &sp : shader_programs)
            sp.second->setMat4("widget", mat);
        widget->render(shader_programs);
    }
}

void GraphStage::addWidget(Widget *widget)
{
    widgets_.push_back(widget);
    if(shown_)
        widget->load();
}

void GraphStage::deleteWidget(Widget *widget)
{
    std::erase(widgets_, widget);
    delete widget;
}

std::list<Widget *> GraphStage::findUnderlyingWidgets(Vec2 pos) const
{
    std::list<Widget*> widgets;
    for(const auto &widget : widgets_)
    {
        Rect trans_bounds = widget->boundingRect();
        trans_bounds.setX(trans_bounds.x() + widget->pos().x);
        trans_bounds.setY(trans_bounds.y() + widget->pos().y);

        if(trans_bounds.x() <= pos.x && trans_bounds.x() + trans_bounds.width() >= pos.x
           && trans_bounds.y() <= pos.y && trans_bounds.y() + trans_bounds.height() >= pos.y)
            widgets.push_back(widget);
    }
    return widgets;
}

std::list<Widget *> GraphStage::maskList(const std::list<Widget *>& source, const std::list<Widget *>& mask) const
{
    std::list<Widget*> widgets;
    bool found;
    for(auto &widget : source)
    {
        found = false;
        for(auto &m_widget : mask)
            if(widget == m_widget)
            {
                found = true;
                break;
            }
        if(!found) widgets.push_back(widget);
    }
    return widgets;
}

std::map<unsigned int, ShaderProgram *> GraphStage::gatherShaderPrograms(unsigned int types)
{
    std::map<unsigned int, ShaderProgram *> programs_;
    for(auto &sp : shader_programs_)
        if(sp.first & types) programs_.insert(sp);
    return programs_;
}

