#ifndef NGE_SCOREBOARD_WIDGET_HPP
#define NGE_SCOREBOARD_WIDGET_HPP


#include "../graph/widget.hpp"
#include "stats_manager.hpp"
#include "../graph/text_widget.hpp"

class ScoreboardWidget : public Widget
{
public:
    ScoreboardWidget(std::vector<StatsEntry> entries, Widget *parent = nullptr);

    void setEntries(std::optional<std::vector<StatsEntry>> entries);

    Rect boundingRect() const override;
    void load() override;
    void unload() override;

private:
    std::vector<TextWidget*> text_labels_;
    TextWidget *title_;
    bool loaded_;
};


#endif //NGE_SCOREBOARD_WIDGET_HPP
