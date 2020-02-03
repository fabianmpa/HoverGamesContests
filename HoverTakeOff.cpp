//
// Simple example to demonstrate how to use the MAVSDK.
//
// Author: Julian Oes <julian@oes.ch>

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

#include <chrono>
#include <cstdint>
#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/action/action.h>
#include <mavsdk/plugins/telemetry/telemetry.h>
#include <iostream>
#include <thread>

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

using namespace mavsdk;
using namespace std::this_thread;
using namespace std::chrono;

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


#define ERROR_CONSOLE_TEXT "\033[31m" // Turn text on console red
#define TELEMETRY_CONSOLE_TEXT "\033[34m" // Turn text on console blue
#define NORMAL_CONSOLE_TEXT "\033[0m" // Restore normal console colour

void usage(std::string bin_name)
{
    std::cout << NORMAL_CONSOLE_TEXT << "Usage : " << bin_name << " <connection_url>" << std::endl
              << "Connection URL format should be :" << std::endl
              << " For TCP : tcp://[server_host][:server_port]" << std::endl
              << " For UDP : udp://[bind_host][:bind_port]" << std::endl
              << " For Serial : serial:///path/to/serial/dev[:baudrate]" << std::endl
              << "For example, to connect to the simulator use URL: udp://:14540" << std::endl;
}

void component_discovered(ComponentType component_type)
{
    std::cout << NORMAL_CONSOLE_TEXT << "Discovered a component with type "
              << unsigned(component_type) << std::endl;
}

//First command: connection type. Second command: camera
int main(int argc, char** argv)
{

     // Create command line parser
    commandLine cmdLine(argc,argv);
      if(cmdLine.GetFlag("help")){
	usage(argv[0]);
	return 1;
     }


    Mavsdk dc;
    std::string connection_url;
    ConnectionResult connection_result;

    bool discovered_system = false;
    if (argc == 3) {
        connection_url = argv[1];
        connection_result = dc.add_any_connection(connection_url);
    } else {
        usage(argv[0]);
        return 1;
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

     printf("\ndetectnet-camera:  successfully initialized camera device\n");
     printf("    width:  %u\n", camera->GetWidth());
     printf("   height:  %u\n", camera->GetHeight());
     printf("    depth:  %u (bpp)\n\n", camera->GetPixelDepth());

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
    /* glDisplay* display = glDisplay::Create();

     if( !display)
     	    printf("detectnet camera: failed to create openGL display \n");  */

    //check camera is opened

    if(!camera -> Open())
    {
	printf("detectnet camera: failed to open camera for streaming \n");
	return 0;
    }

    printf("detectnet camera: camera open for streaming \n");

    //float confidence = 0.0f; 

    //End of camera initialization


    if (connection_result != ConnectionResult::SUCCESS) {
        std::cout << ERROR_CONSOLE_TEXT
                  << "Connection failed: " << connection_result_str(connection_result)
                  << NORMAL_CONSOLE_TEXT << std::endl;
        return 1;
    }

    // We don't need to specify the UUID if it's only one system anyway.
    // If there were multiple, we could specify it with:
    // dc.system(uint64_t uuid);
    System& system = dc.system();

    std::cout << "Waiting to discover system..." << std::endl;
    dc.register_on_discover([&discovered_system](uint64_t uuid) {
        std::cout << "Discovered system with UUID: " << uuid << std::endl;
        discovered_system = true;
    });

    // We usually receive heartbeats at 1Hz, therefore we should find a system after around 2
    // seconds.
    sleep_for(seconds(2));

    if (!discovered_system) {
        std::cout << ERROR_CONSOLE_TEXT << "No system found, exiting." << NORMAL_CONSOLE_TEXT
                  << std::endl;
        return 1;
    }

    // Register a callback so we get told when components (camera, gimbal) etc
    // are found.
    system.register_component_discovered_callback(component_discovered);

    auto telemetry = std::make_shared<Telemetry>(system);
    auto action = std::make_shared<Action>(system);

    // We want to listen to the altitude of the drone at 1 Hz.
    const Telemetry::Result set_rate_result = telemetry->set_rate_position(1.0);
    if (set_rate_result != Telemetry::Result::SUCCESS) {
        std::cout << ERROR_CONSOLE_TEXT
                  << "Setting rate failed:" << Telemetry::result_str(set_rate_result)
                  << NORMAL_CONSOLE_TEXT << std::endl;
        return 1;
    }

    // Set up callback to monitor altitude while the vehicle is in flight
    telemetry->position_async([](Telemetry::Position position) {
        std::cout << TELEMETRY_CONSOLE_TEXT // set to blue
                  << "Altitude: " << position.relative_altitude_m << " m"
                  << NORMAL_CONSOLE_TEXT // set to default color again
                  << std::endl;
    });

    // Check if vehicle is ready to arm
    while (telemetry->health_all_ok() != true) {
        std::cout << "Vehicle is getting ready to arm" << std::endl;
        sleep_for(seconds(1));
    }

    // Arm vehicle
    std::cout << "Arming..." << std::endl;
    const Action::Result arm_result = action->arm();

    if (arm_result != Action::Result::SUCCESS) {
        std::cout << ERROR_CONSOLE_TEXT << "Arming failed:" << Action::result_str(arm_result)
                  << NORMAL_CONSOLE_TEXT << std::endl;
        return 1;
    }

    // Take off
    std::cout << "Taking off..." << std::endl;
    const Action::Result takeoff_result = action->takeoff();
    if (takeoff_result != Action::Result::SUCCESS) {
        std::cout << ERROR_CONSOLE_TEXT << "Takeoff failed:" << Action::result_str(takeoff_result)
                  << NORMAL_CONSOLE_TEXT << std::endl;
        return 1;
    }

    
    int count = 0;	
    while(count!=1000) 
  {
      
      count++;
	
      float* imgRGBA = NULL;

      if(!camera->CaptureRGBA(&imgRGBA,1000))
   		printf("detectnet camera: failed to capture RGBA image from camera \n");

      detectNet::Detection* detections = NULL;

      const int numDetections = net->Detect(imgRGBA, camera -> GetWidth(), camera -> GetHeight(), &detections, overlayFlags);

      if(numDetections > 0)
      {
     		/* printf(" %i onjects detected \n", numDetections);

		for( int n=0; n < numDetections; n++ )
		{
		printf("detected obj %i  class #%u (%s)  confidence=%f\n", n, detections[n].ClassID, net->GetClassDesc(detections[n].ClassID), detections[n].Confidence);
		printf("bounding box %i (%f,%f) (%f,%f) w=%f h=%f \n",n , detections[n].Left, detections[n].Top, detections[n].Right, detections[n].Bottom,detections[n].Width(), detections[n].Height()); 
		} */ 
      }	
      /*
      // update display
      if( display != NULL )
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
      }    */

      // print out timing info
      net->PrintProfilerTimes(); 
      enc->EncodeRGBA(imgRGBA);
  
      //sleep_for(seconds(1));
  }
 	
    // Let it hover for a bit before landing again.
    //sleep_for(seconds(10));

    std::cout << "Landing..." << std::endl;
    const Action::Result land_result = action->land();
    if (land_result != Action::Result::SUCCESS) {
        std::cout << ERROR_CONSOLE_TEXT << "Land failed:" << Action::result_str(land_result)
                  << NORMAL_CONSOLE_TEXT << std::endl;
        return 1;
    }

    // Check if vehicle is still in air
    while (telemetry->in_air()) {
        std::cout << "Vehicle is landing..." << std::endl;
        sleep_for(seconds(1));
    }
    std::cout << "Landed!" << std::endl;

    // We are relying on auto-disarming but let's keep watching the telemetry for a bit longer.
    sleep_for(seconds(3));
    std::cout << "Finished..." << std::endl;

    SAFE_DELETE(camera);
    //SAFE_DELETE(display);
    SAFE_DELETE(net);
    SAFE_DELETE(enc);

    printf("detectnet-camera:  shutdown complete.\n");
    return 0;
}
