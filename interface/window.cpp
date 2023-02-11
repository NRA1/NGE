#include "window.hpp"

#include <utility>

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

    while (!native_->shouldClose())
    {
        native_->poolEvents();
        if(stage_ != nullptr) stage_->render();
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

