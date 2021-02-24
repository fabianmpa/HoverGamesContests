#include "CameraHandler.hpp"

void CameraHandler::StandbyState(void){}
void CameraHandler::PreactiveState(void){}

void CameraHandler::RecordVideo(void)
{
	std::shared_ptr<cv::Mat> frame = vReader.ReadFrame();
	vWriter.WriteFrame(frame);
}

void CameraHandler::CaptureImages(void){}
void CameraHandler::PausedState(void){}
void CameraHandler::FaultedState(void){}

void CameraHandler::InitReader()
{
	vReader.Init("/dev/video2");
}

void CameraHandler::InitWriter()
{
	vWriter.Init("test.avi", 25, vReader.GetSourceWidth(), vReader.GetSourceHeight());
}

CameraHandler::~CameraHandler()
{
	vWriter.Close();
	vReader.Close();
}

void CameraHandler::StateMachine()
{
	for (;;)
	{
		switch (currentHovergames.HoverGames_SM)
		{
		case HOVERGAMES_STANDBY_STATE: /*Sandby State*/
			StandbyState();
			break;
		case HOVERGAMES_PREACTIVE_STATE: /*PreActive State*/
			PreactiveState();
			break;
		case HOVERGAMES_ACTIVE_STATE: /*Active State*/
			switch (currentHovergames.HoverGames_ActiveSM)
			{
			case HOVERGAMES_RECORDING_STATE: /*Recording State*/
				RecordVideo();
				break;
			case HOVERGAMES_IMAGE_CAPTURE_STATE: /*Image Capture State*/
				CaptureImages();
				break;
			case HOVERGAMES_PAUSED_STATE: /*Paused State*/
				PausedState();
				break;
			default:
				/*Do nothing*/
				break;
			}
			break;
		case HOVERGAMES_FAULTED_STATE: /*Faulted State*/
			FaultedState();
			break;
		default:
			/* Do nothing*/
			break;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(40));
	}
}