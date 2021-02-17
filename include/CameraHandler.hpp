#include <opencv2/opencv.hpp>
#include "Types.hpp"
#define HOVERGAMES_STANDBY_STATE        0U
#define HOVERGAMES_PREACTIVE_STATE      1U
#define HOVERGAMES_ACTIVE_STATE         2U
#define HOVERGAMES_FAULTED_STATE        3U
#define HOVERGAMES_RECORDING_STATE      0U
#define HOVERGAMES_IMAGE_CAPTURE_STATE  1U
#define HOVERGAMES_PAUSED_STATE         2U

using namespace cv;

class CameraHandler
{
public:
    CameraHandler()=default;
    ~CameraHandler();
    void StandbyState(void);
    void PreactiveState(void);
    void ActiveState(void);
    void RecordVideo(void);
    void CaptureImages(void);
    void FaultedState(void);
    void PausedState(void);
    void StateMachine(void);
    std::function<void(Telemetry::HoverGamesStatus_s)> StatusCallback = [this](Telemetry::HoverGamesStatus_s hgStatus) {
        currentHovergames.HoverGames_SM = hgStatus.HoverGames_SM;
        currentHovergames.HoverGames_ActiveSM = hgStatus.HoverGames_ActiveSM;
    };

private:
    Mat frame;
    hovergames_s currentHovergames;
    VideoCapture video; /*Web camera for testing purposes*/
    VideoWriter sink;
};
