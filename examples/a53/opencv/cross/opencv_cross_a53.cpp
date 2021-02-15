#include <opencv2/opencv.hpp>
#include <iostream> 
using namespace std;
using namespace cv; 
int main(){
	// Create a VideoCapture object and use camera to capture the video
	//VideoCapture cap("v4l2src device=/dev/video0 ! vpuenc_h264 ! avimux ! appsink",CAP_GSTREAMER);
        VideoCapture cap("/dev/video0");
	// Check if camera opened successfully
	if(!cap.isOpened())
	{
		cout << "Error opening video stream" << endl; 
		return -1; 
	} 
	// Default resolution of the frame is obtained.The default resolution is system dependent. 
	int frame_width = cap.get(CAP_PROP_FRAME_WIDTH); 
	int frame_height = cap.get(CAP_PROP_FRAME_HEIGHT); 
	// Define the codec and create VideoWriter object.The output is stored in 'outcpp.avi' file. 
	VideoWriter video("test.avi",VideoWriter::fourcc('X','2','6','4'),30, Size(frame_width,frame_height),true);
        if(!video.isOpened())
        {
		std::cout<<"VideoWriter not opened";
		return -1;
	}
 
	while(1)
	{ 
		Mat frame; 
		// Capture frame-by-frame 
		cap >> frame;
		// If the frame is empty, break immediately
		if (frame.empty())
			break;
		// Write the frame into the file 'outcpp.avi'
		video.write(frame);
		// Press  ESC on keyboard to  exit

	}
	// When everything done, release the video capture and write object
	cap.release();
	video.release();
	// Closes all the windows
	return 0;
}
