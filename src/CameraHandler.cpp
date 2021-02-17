#include "CameraHandler.hpp"

void CameraHandler::StandbyState(void){}
void CameraHandler::PreactiveState(void){}
void CameraHandler::ActiveState(void){}
void CameraHandler::RecordVideo(void){}
void CameraHandler::CaptureImages(void){}
void CameraHandler::PausedState(void){}
void CameraHandler::FaultedState(void){}

CameraHandler::~CameraHandler()
{
	/*Free memory of both objects*/
	video.release();
	sink.release();
}

void CameraHandler::StateMachine()
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
		ActiveState();
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
}