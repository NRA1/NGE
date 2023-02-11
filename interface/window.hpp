#ifndef NGE_WINDOW_HPP
#define NGE_WINDOW_HPP


#include <string>
#include "../infrastructure/logger.hpp"
#include "../infrastructure/game.hpp"
#include "../data_objects/size.hpp"
#include "../native/x_11_window.hpp"
#include "abstract_stage.hpp"

class GameWindow
{
public:
    GameWindow(std::string title, int width, int height, int state);
    void exec();
    void setStage(AbstractStage *stage);

    ~GameWindow();

private:
    void viewportSizeChanged(Size size);
    void eventHandler(Event *event);

    AbstractNativeWindow<GameWindow> *native_;

    std::string title_;
    Size size_;
    AbstractStage *stage_;

    bool shown_{};
};

#endif //NGE_WINDOW_HPP
