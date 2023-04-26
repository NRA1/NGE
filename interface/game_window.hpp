#ifndef NGE_WINDOW_HPP
#define NGE_WINDOW_HPP

#include <string>
#include <queue>
#include <chrono>
#include <utility>
#include "../infrastructure/logger.hpp"
#include "../infrastructure/game.hpp"
#include "../data_objects/size.hpp"
#include "../native/x_11_window.hpp"
#include "abstract_stage.hpp"
#include "../native/abstract_native_window.hpp"

class GameWindow
{
public:
    GameWindow(std::string title, int width, int height, int state);
    void exec();
    void pushStage(AbstractStage *stage);
    void insertStage(AbstractStage *stage, unsigned int index);
    void eraseStage(AbstractStage *stage);
    void disposeStage(AbstractStage *stage);
    void setCursorVisibility(bool visible);
    bool isCursorVisible();
    void close();

    ~GameWindow();

private:
    struct StageAction
    {
        enum
        {
            Dispose,
            Insert,
            Push,
            Erase
        } Action;

        AbstractStage *Stage;
        unsigned int Index;
    };

    void viewportSizeChanged(Size size);
    void eventHandler(Event *event);
    void applyDifferedActions();
    void differedEraseStage(AbstractStage *stage);
    void differedInsertStage(AbstractStage *stage, unsigned int index);

    AbstractNativeWindow<GameWindow> *native_;

    std::string title_;
    Size size_;
    std::vector<AbstractStage *> stages_;

    std::queue<StageAction> differ_queue_;

    bool shown_;
};

#endif //NGE_WINDOW_HPP
