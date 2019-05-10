#include"Get_bmpdata.h"
#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include <fstream>              // File IO
#include <iostream>             // Terminal IO
#include <sstream>              // Stringstreams
#include<stdio.h>
// 3rd party header for writing png files
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int get_bmp()
{

rs2::colorizer color_map;

    // Declare RealSense pipeline, encapsulating the actual device and sensors
    rs2::pipeline pipe;
    // Start streaming with default recommended configuration
    pipe.start();

    // Capture 30 frames to give autoexposure, etc. a chance to settle
	

		// Wait for the next set of frames from the camera. Now that autoexposure, etc.
		// has settled, we will write these to disk
		for (auto&& frame : pipe.wait_for_frames())
		{
			// We can only save video frames as pngs, so we skip the rest
			if (auto vf = frame.as<rs2::video_frame>())
			{
				auto stream = frame.get_profile().stream_type();
				// Use the colorizer to get an rgb image for the depth stream
				if (vf.is<rs2::depth_frame>()) vf = color_map.process(frame);

				// Write images to disk

				stbi_write_bmp("yk.bmp", vf.get_width(), vf.get_height(),
					vf.get_bytes_per_pixel(), vf.get_data());


				// Record per-frame metadata for UVC streams

			}
		}

		
	

    return EXIT_SUCCESS;


}