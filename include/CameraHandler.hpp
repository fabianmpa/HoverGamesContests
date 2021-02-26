#include "VideoWriter.hpp"
#include "VideoReader.hpp"
#include "Types.hpp"
#define HOVERGAMES_STANDBY_STATE        0U
#define HOVERGAMES_PREACTIVE_STATE      1U
#define HOVERGAMES_ACTIVE_STATE         2U
#define HOVERGAMES_FAULTED_STATE        3U
#define HOVERGAMES_RECORDING_STATE      0U
#define HOVERGAMES_IMAGE_CAPTURE_STATE  1U
#define HOVERGAMES_PAUSED_STATE         2U

class CameraHandler
{
public:
    CameraHandler(string& device): source{device}{};
    ~CameraHandler();
    /*Open Video Capture Object*/
    void InitReader(void);
    /*Open VideoWriter object*/
    void InitWriter(void);
    /*Main state machine which decides what the state of the camera is*/
    void StateMachine(void);
    /*Lambda callback which updates the value of hovermaes message comming from mavlink */

private:
    void StandbyState(void);
    void PreactiveState(void);
    void RecordVideo(void);
    void CaptureImages(void);
    void FaultedState(void);
    void PausedState(void);
    VideoReader vReader;
    Video_Writer vWriter;
    mutex stateMachineMutex;
    hovergames_s currentHovergames;

protected:
    string source;
    string sink   {"hovergames.avi"};
    double fps    {15.0F};
};
