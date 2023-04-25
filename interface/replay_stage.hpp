#ifndef NGE_REPLAY_STAGE_HPP
#define NGE_REPLAY_STAGE_HPP


#include "abstract_stage.hpp"
#include "../world/object.hpp"
#include "../world/camera.hpp"
#include "../world/ground.hpp"
#include "world_stage.hpp"
#include "../data_objects/track_stamp.hpp"

class ReplayStage : public WorldStage
{
public:
    explicit ReplayStage(std::string track_path);

    bool handleEvent(Event *event) override;
    void render(unsigned int time) override;

    void onAppearing() override;
    void onDisappearing() override;


    void setObject(Object *object);
    void setInputDelegate(bool (*inputDelegate)(ReplayStage *, Event *));
    void setFinishedDelegate(void (*finishedDelegate)(ReplayStage *));

    ~ReplayStage() override;

private:
    void tick(unsigned int time);

    Object *object_;

    std::string track_file_path_;
    std::ifstream track_file_;

    std::optional<TrackStamp> last_stamp_;

    bool (*input_delegate_)(ReplayStage *stage, Event *event);
    void (*finished_delegate_)(ReplayStage *stage);
};


#endif //NGE_REPLAY_STAGE_HPP
