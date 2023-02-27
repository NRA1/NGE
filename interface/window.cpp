#include "window.hpp"

GameWindow::GameWindow(std::string title, int width, int height, int) :  native_(nullptr), title_(std::move(title)),
                                                                         size_(width, height), stage_(nullptr), shown_(false)
{
    if(!Game::initialized())
    {
        log() - Fatal < "Game not initialized";
        Game::fault();
    }

#ifdef __unix__
    native_ = new X11Window<GameWindow>(title_, size_);
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
    if(stage_ != nullptr)
    {
        stage_->viewportSizeChanged(native_->size());
        stage_->onAppearing();
    }

    unsigned int last_render = duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()
            .time_since_epoch()).count();
    while (!native_->shouldClose())
    {
        native_->poolEvents();
        unsigned int time = duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()
                .time_since_epoch()).count();
        if(stage_ != nullptr) stage_->render(time - last_render);
        last_render = time;
        native_->swapBuffers();
    }
    shown_ = false;
    if(stage_ != nullptr) stage_->onDisappearing();
}

void GameWindow::setStage(AbstractStage *stage)
{
    if(stage_ != nullptr && shown_) stage_->onDisappearing();
    delete stage_;
    stage_ = stage;
    if(stage_ != nullptr && shown_)
    {
        stage->viewportSizeChanged(native_->size());
        stage->onAppearing();
    }
}

void GameWindow::viewportSizeChanged(Size size)
{
    if(stage_ != nullptr)
        stage_->viewportSizeChanged(size);
}

void GameWindow::eventHandler(Event *event)
{
    if(stage_ == nullptr)
    {
        log() - Debug < "No stage set, ignoring event";
        return;
    }

    stage_->handleEvent(event);
}

GameWindow::~GameWindow()
{
    delete stage_;
}

void GameWindow::setCursorVisibility(bool visible)
{
    return native_->setCursorVisibility(visible);
}

bool GameWindow::isCursorVisible()
{
    return native_->cursorVisible();
}

