/**
 * @file fly_qgc_mission.cpp
 *
 * @brief Demonstrates how to import mission items from QGroundControl plan,
 * and fly them using the MAVSDK.
 *
 * Steps to run this example:
 * 1. (a) Create a Mission in QGroundControl and save them to a file (.plan) (OR)
 *    (b) Use a pre-created sample mission plan in "plugins/mission/qgroundcontrol_sample.plan".
 *    Click
 * [here](https://user-images.githubusercontent.com/26615772/31763673-972c5bb6-b4dc-11e7-8ff0-f8b39b6b88c3.png)
 * to see what sample mission plan in QGroundControl looks like.
 * 2. Run the example by passing path of the QGC mission plan as argument (By default, sample
 * mission plan is imported).
 *
 * Example description:
 * 1. Imports QGC mission items from .plan file.
 * 2. Uploads mission items to vehicle.
 * 3. Starts mission from first mission item.
 * 4. Commands RTL once QGC Mission is accomplished.
 *
 * @author Shakthi Prashanth M <shakthi.prashanth.m@intel.com>,
 *         Julian Oes <julian@oes.ch>
 * @date 2018-02-04
 */


/*
 * Copyright (c) 2017, NVIDIA CORPORATION. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

//MAVSDK
#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/action/action.h>
#include <mavsdk/plugins/mission/mission.h>
#include <mavsdk/plugins/telemetry/telemetry.h>
#include <functional>
#include <future>
#include <iostream>
#include <memory>

//jetson-inference
#include <jetson-utils/gstCamera.h>
#include <jetson-utils/glDisplay.h>
#include <jetson-inference/detectNet.h>
#include <jetson-utils/commandLine.h>
#include <signal.h>
#include <iostream>
#include <jetson-utils/cudaNormalize.h>
#include <jetson-utils/gstUtility.h>
#include <jetson-utils/gstEncoder.h>

#define ERROR_CONSOLE_TEXT "\033[31m" // Turn text on console red
#define TELEMETRY_CONSOLE_TEXT "\033[34m" // Turn text on console blue
#define NORMAL_CONSOLE_TEXT "\033[0m" // Restore normal console colour

using namespace mavsdk;
using namespace std::chrono; // for seconds(), milliseconds()
using namespace std::this_thread; // for sleep_for()

using namespace std;

bool signal_recieved = false;

void sig_handler(int signo)
{
	if(signo == SIGINT)
	{
		printf("received SIGINT \n");
		signal_recieved = true;
	}
}


// Handles Action's result
inline void handle_action_err_exit(Action::Result result, const std::string& message);
// Handles Mission's result
inline void handle_mission_err_exit(Mission::Result result, const std::string& message);
// Handles Connection result
inline void handle_connection_err_exit(ConnectionResult result, const std::string& message);

void usage(std::string bin_name)
{
    std::cout << NORMAL_CONSOLE_TEXT << "Usage : " << bin_name
              << " <connection_url> [path of QGC Mission plan]" << std::endl
              << "Connection URL format should be :" << std::endl
              << " For TCP : tcp://[server_host][:server_port]" << std::endl
              << " For UDP : udp://[bind_host][:bind_port]" << std::endl
              << " For Serial : serial:///path/to/serial/dev[:baudrate]" << std::endl
              << "For example, to connect to the simulator use URL: udp://:14540" << std::endl;
}

// Note: Firt arg: connection. Second: plan. Third: camera
int main(int argc, char** argv)
{
    // Create command line parser
    commandLine cmdLine(argc,argv);
      if(cmdLine.GetFlag("help")){
	usage(argv[0]);
	return 1;
     }

    // Create MAVSDK object, connection and connection result
    Mavsdk dc;
    std::string connection_url;
    ConnectionResult connection_result;

    // Locate path of QGC Sample plan
    std::string qgc_plan = "../../../plugins/mission/qgroundcontrol_sample.plan";

    if (argc!=4) {
        usage(argv[0]);
        return 1;
    }

    //set curl from first
    connection_url = argv[1];

    //set plan from second argument
    if (argc >= 3) {
        qgc_plan = argv[2];
    }

    //create camera device
    gstCamera* camera = gstCamera::Create(cmdLine.GetInt("width", gstCamera::DefaultWidth), cmdLine.GetInt("height", gstCamera::DefaultHeight), cmdLine.GetString("camera"));

    //Create Encoder
    gstEncoder* enc = gstEncoder::Create(GST_CODEC_H265,1280,720,"video.mp4");

    if( !camera )
    {
	printf("\n Detectnet-camera: failed to initiallize camera device \n");
	return 0;
    }
	/*
     printf("\ndetectnet-camera:  successfully initialized camera device\n");
     printf("    width:  %u\n", camera->GetWidth());
     printf("   height:  %u\n", camera->GetHeight());
     printf("    depth:  %u (bpp)\n\n", camera->GetPixelDepth());
	*/

     //Create detection network

     detectNet* net= detectNet::Create(argc,argv);

     if(!net)
     { 
	printf("detectnet-camera: failed to load detectnet model \n");
	return 0;
     }

     //parse overlay flags
     
     const uint32_t overlayFlags = detectNet::OverlayFlagsFromStr(cmdLine.GetString("overlay","box,labels,conf"));
	

     //create openGL window
     //remove when execute in drone
     //glDisplay* display = glDisplay::Create();

    // if( !display)
     	  //  printf("detectnet camera: failed to create openGL display \n");

    //check camera is opened

    if(!camera -> Open())
    {
	printf("detectnet camera: failed to open camera for streaming \n");
	return 0;
    }

    printf("detectnet camera: camera open for streaming \n");

    //float confidence = 0.0f; 

    //End of camera initialization

    std::cout << "Connection URL: " << connection_url << std::endl;
    std::cout << "Importing mission from mission plan: " << qgc_plan << std::endl;

    {
        auto prom = std::make_shared<std::promise<void>>();
        auto future_result = prom->get_future();

        std::cout << "Waiting to discover system..." << std::endl;
        dc.register_on_discover([prom](uint64_t uuid) {
            std::cout << "Discovered system with UUID: " << uuid << std::endl;
            prom->set_value();
        });

        connection_result = dc.add_any_connection(connection_url);
        handle_connection_err_exit(connection_result, "Connection failed: ");

        future_result.get();
    }

    dc.register_on_timeout([](uint64_t uuid) {
        std::cout << "System with UUID timed out: " << uuid << std::endl;
        std::cout << "Exiting." << std::endl;
        exit(0);
    });

    // We don't need to specify the UUID if it's only one system anyway.
    // If there were multiple, we could specify it with:
    // dc.system(uint64_t uuid);
    System& system = dc.system();
    auto action = std::make_shared<Action>(system);
    auto mission = std::make_shared<Mission>(system);
    auto telemetry = std::make_shared<Telemetry>(system);

    while (!telemetry->health_all_ok()) {
        std::cout << "Waiting for system to be ready" << std::endl;
        sleep_for(seconds(2));
    }

    std::cout << "System ready" << std::endl;

    // Import Mission items from QGC plan
    Mission::mission_items_t mission_items;
    Mission::Result import_res = Mission::import_qgroundcontrol_mission(mission_items, qgc_plan);
    handle_mission_err_exit(import_res, "Failed to import mission items: ");

    if (mission_items.size() == 0) {
        std::cerr << "No missions! Exiting..." << std::endl;
	SAFE_DELETE(camera);
    	//SAFE_DELETE(display);
    	SAFE_DELETE(net);
    	SAFE_DELETE(enc);
        exit(EXIT_FAILURE);
    }
    std::cout << "Found " << mission_items.size() << " mission items in the given QGC plan."
              << std::endl;

    {
        std::cout << "Uploading mission..." << std::endl;
        // Wrap the asynchronous upload_mission function using std::future.
        auto prom = std::make_shared<std::promise<Mission::Result>>();
        auto future_result = prom->get_future();
        mission->upload_mission_async(
            mission_items, [prom](Mission::Result result) { prom->set_value(result); });

        const Mission::Result result = future_result.get();
        handle_mission_err_exit(result, "Mission upload failed: ");
        std::cout << "Mission uploaded." << std::endl;
    }

    std::cout << "Arming..." << std::endl;
    const Action::Result arm_result = action->arm();
    handle_action_err_exit(arm_result, "Arm failed: ");
    std::cout << "Armed." << std::endl;

    // Before starting the mission subscribe to the mission progress.
    mission->subscribe_progress([](int current, int total) {
        std::cout << "Mission status update: " << current << " / " << total << std::endl;
    });

    {
        std::cout << "Starting mission." << std::endl;
        auto prom = std::make_shared<std::promise<Mission::Result>>();
        auto future_result = prom->get_future();
        mission->start_mission_async([prom](Mission::Result result) {
            prom->set_value(result);
            std::cout << "Started mission." << std::endl;
        });

        const Mission::Result result = future_result.get();
        handle_mission_err_exit(result, "Mission start failed: ");
    }

    while (!mission->mission_finished()) 
  {
      
      float* imgRGBA = NULL;

      if(!camera->CaptureRGBA(&imgRGBA,1000))
   		printf("detectnet camera: failed to capture RGBA image from camera \n");

      detectNet::Detection* detections = NULL;

      const int numDetections = net->Detect(imgRGBA, camera -> GetWidth(), camera -> GetHeight(), &detections, overlayFlags);

      if(numDetections > 0)
      {
	/*
     		printf(" %i onjects detected \n", numDetections);

		for( int n=0; n < numDetections; n++ )
		{
		printf("detected obj %i  class #%u (%s)  confidence=%f\n", n, detections[n].ClassID, net->GetClassDesc(detections[n].ClassID), detections[n].Confidence);
		printf("bounding box %i (%f,%f) (%f,%f) w=%f h=%f \n",n , detections[n].Left, detections[n].Top, detections[n].Right, detections[n].Bottom,detections[n].Width(), detections[n].Height()); 
		}   */
      }	

      // update display
   /*   if( display != NULL )
      {
	// render the image
	display->RenderOnce(imgRGBA, camera->GetWidth(), camera->GetHeight());

	// update the status bar
	char str[256];
	sprintf(str, "TensorRT %i.%i.%i | %s | Network %.0f FPS", NV_TENSORRT_MAJOR, NV_TENSORRT_MINOR, NV_TENSORRT_PATCH, precisionTypeToStr(net->GetPrecision()), net->GetNetworkFPS());
	display->SetTitle(str);

	// check if the user quit
	if( display->IsClosed() )
	signal_recieved = true;
      }   */

      // print out timing info
      net->PrintProfilerTimes(); 
      enc->EncodeRGBA(imgRGBA);
  
      //sleep_for(seconds(1));
  }
 
    // Wait for some time.
    sleep_for(seconds(5));

    {
        // Mission complete. Command RTL to go home.
        std::cout << "Commanding RTL..." << std::endl;
        const Action::Result result = action->return_to_launch();
        if (result != Action::Result::SUCCESS) {
            std::cout << "Failed to command RTL (" << Action::result_str(result) << ")"
                      << std::endl;
        } else {
            std::cout << "Commanded RTL." << std::endl;
        }
    }

    //destroy resources
    printf("detectnet-camera:  shutting down...\n");
	
    SAFE_DELETE(camera);
    //SAFE_DELETE(display);
    SAFE_DELETE(net);
    SAFE_DELETE(enc);

    printf("detectnet-camera:  shutdown complete.\n");
    return 0;


}

inline void handle_action_err_exit(Action::Result result, const std::string& message)
{
    if (result != Action::Result::SUCCESS) {
        std::cerr << ERROR_CONSOLE_TEXT << message << Action::result_str(result)
                  << NORMAL_CONSOLE_TEXT << std::endl;
        exit(EXIT_FAILURE);
    }
}

inline void handle_mission_err_exit(Mission::Result result, const std::string& message)
{
    if (result != Mission::Result::SUCCESS) {
        std::cerr << ERROR_CONSOLE_TEXT << message << Mission::result_str(result)
                  << NORMAL_CONSOLE_TEXT << std::endl;
        exit(EXIT_FAILURE);
    }
}

// Handles connection result
inline void handle_connection_err_exit(ConnectionResult result, const std::string& message)
{
    if (result != ConnectionResult::SUCCESS) {
        std::cerr << ERROR_CONSOLE_TEXT << message << connection_result_str(result)
                  << NORMAL_CONSOLE_TEXT << std::endl;
        exit(EXIT_FAILURE);
    }
}
