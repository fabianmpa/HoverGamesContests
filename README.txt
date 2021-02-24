Code for HoverGames2 and subequent contests

For the A53 core:
- receive mavlink messages from flight controller FMU K66
	-Hovergames State machine module created in PX4
	- GPS data
	- Mission Status
- Activate or deactivate tracking algorithm
	- The state machine defined in the CameraHandler Class is the one that decides whether the camera is activated or not
	- The tracking algorithm should run in Active State
- Store GPS data to add it to the Android application
	- GpsHandler stores the data in a GeoJSON format, which then is loaded to the Maps Application
