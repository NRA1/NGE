#include "game_window.hpp"
#include "../native/win_32_window.hpp"

GameWindow::GameWindow(std::string title, int width, int height, int) : native_(nullptr), title_(std::move(title)),
                                                                        size_((float)width, (float)height), shown_(false)
{
    if (!Game::initialized())
    {
        log() - Fatal < "Game not initialized";
        Game::fault();
    }

#ifdef __unix__
        native_ = new X11Window<GameWindow>(title_, size_);
#endif
#ifdef WIN32
    native_ = new Win32Window<GameWindow>(title_, size_);
#endif
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDebugMessageCallback(&Game::openglDebugMessageCallback, 0);

    native_->setSubscriber(this);
    native_->setViewportChangeHandler(&GameWindow::viewportSizeChanged);
    native_->setEventHandler(&GameWindow::eventHandler);
}

void GameWindow::exec()
{
    native_->show();
    shown_ = true;

    viewportSizeChanged(native_->size());
    for(auto &stage : stages_)
        stage->onAppearing();

    unsigned int last_render = (unsigned int)duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()
            .time_since_epoch()).count();
    while (!native_->shouldClose())
    {
        native_->poolEvents();
        unsigned int time = (unsigned int)duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()
                .time_since_epoch()).count();
        for(auto &stage : stages_)
            stage->render(time - last_render);
        last_render = time;
        native_->swapBuffers();
    }
    shown_ = false;
    for(auto &stage : stages_)
        stage->onDisappearing();
}

void GameWindow::pushStage(AbstractStage *stage)
{
    stages_.push_back(stage);
    if(shown_)
    {
        stage->viewportSizeChanged(native_->size());
        stage->onAppearing();
    }
}

void GameWindow::viewportSizeChanged(Size size)
{
    for(auto &stage : stages_)
        stage->viewportSizeChanged(size);
}

void GameWindow::eventHandler(Event *event)
{
    if(stages_.empty())
    {
        log() - Debug < "No stage set, ignoring event";
        return;
    }

    for(long i = (long)stages_.size() - 1; i >= 0; i--)
        if(stages_[i]->handleEvent(event)) return;
}

GameWindow::~GameWindow()
{
    for(auto &stage : stages_)
        delete stage;
}

void GameWindow::setCursorVisibility(bool visible)
{
    return native_->setCursorVisibility(visible);
}

bool GameWindow::isCursorVisible()
{
    return native_->cursorVisible();
}

void GameWindow::close()
{
    native_->setShouldClose(true);
}

void GameWindow::eraseStage(AbstractStage *stage)
{
    if(shown_)
    {
        stage->onDisappearing();
    }
    erase_if(stages_, [stage](AbstractStage *x) { return x == stage; });
}

void GameWindow::insertStage(AbstractStage *stage, unsigned int index)
{
    auto it = stages_.begin();
    for(unsigned int i = 0; i < index; i++)
        it++;
    stages_.insert(it, stage);
    if(shown_)
    {
        stage->viewportSizeChanged(native_->size());
        stage->onAppearing();
    }
}

