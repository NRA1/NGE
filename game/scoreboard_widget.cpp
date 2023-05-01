#include "scoreboard_widget.hpp"

ScoreboardWidget::ScoreboardWidget(std::vector<StatsEntry> entries, Widget *parent)
        : Widget(parent)
{
    setEntries(entries);
}

void ScoreboardWidget::setEntries(std::optional<std::vector<StatsEntry>> entries)
{
    deleteChildren();
    title_ = new TextWidget("Scoreboard", ":/fonts/arial.ttf", 75, Vec4(1, 1, 1, 1), this);
    title_->setPos(Vec2(-200, 225));

    if(entries.has_value())
    {
        for (unsigned int i = 0; i < entries->size(); ++i)
        {
            int vpos = (((int) entries->size() * 50) / 2) - ((int) (i + 1) * 50);
            TextWidget *player = new TextWidget(entries->at(i).Player, ":/fonts/arial.ttf", 40, Vec4(1, 1, 1, 1), this);
            player->setPos(Vec2(-400, vpos));
            TextWidget *points = new TextWidget(std::to_string(entries->at(i).Points), ":/fonts/arial.ttf", 40,
                                                Vec4(1, 1, 1, 1), this);
            points->setPos(Vec2(100, vpos));
            TextWidget *level = new TextWidget(std::to_string(entries->at(i).Level), ":/fonts/arial.ttf", 40,
                                               Vec4(1, 1, 1, 1), this);
            level->setPos(Vec2(300, vpos));
        }
        loadChildren();
    }
}

Rect ScoreboardWidget::boundingRect() const
{
    return Rect();
}

void ScoreboardWidget::load()
{
    loaded_ = true;
}

void ScoreboardWidget::unload()
{
    loaded_ = false;
}
