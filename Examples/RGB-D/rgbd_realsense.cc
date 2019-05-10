/**
* This file is part of ORB-SLAM2.
*
* Copyright (C) 2014-2016 Raúl Mur-Artal <raulmur at unizar dot es> (University of Zaragoza)
* For more information see <https://github.com/raulmur/ORB_SLAM2>
*
* ORB-SLAM2 is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* ORB-SLAM2 is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with ORB-SLAM2. If not, see <http://www.gnu.org/licenses/>.
*/


#include<iostream>
#include<algorithm>
#include<fstream>
#include<chrono>

#include<opencv2/core/core.hpp>

#include<System.h>

#include <iostream>
#include <librealsense2/rs.hpp>
#include "cv-helpers.hpp"

using namespace std;
using namespace std::chrono;

int main(int argc, char **argv)
{
    if(argc != 3)
    {
        cerr << endl << "Usage: ./rgbd_realsense path_to_vocabulary path_to_settings" << endl;
        return 1;
    }

    rs2::log_to_console(RS2_LOG_SEVERITY_DEBUG);

    rs2::pipeline p;
    rs2::config cfg;
    cfg.enable_stream(RS2_STREAM_DEPTH, 640, 480, RS2_FORMAT_Z16, 30);
    cfg.enable_stream(RS2_STREAM_COLOR, 640, 480, RS2_FORMAT_BGR8, 30);

    // start realsense
    auto profile = p.start(cfg);
    rs2::align align_to(RS2_STREAM_COLOR);

    // Create SLAM system. It initializes all system threads and gets ready to process frames.
    ORB_SLAM2::System SLAM(argv[1],argv[2],ORB_SLAM2::System::RGBD,true);

    // Main loop
    cv::Mat imRGB, imD;
    while(true)
    {
        char ch = cv::waitKey(10);
        if (ch == 27) {
          break;
        }
	std::cout << "1\n";
        rs2::frameset frames = p.wait_for_frames();
	std::cout << "2\n";
        double tframe = time_point_cast<nanoseconds>(steady_clock::now()).time_since_epoch().count();
        frames = align_to.process(frames);
        auto color = frames.get_color_frame();
        auto depth = frames.get_depth_frame();
        static int last_frame_number = 0;
        if (color.get_frame_number() == last_frame_number) continue;
        last_frame_number = color.get_frame_number();

        auto imRGB = frame_to_mat(color);
        auto imD = depth_frame_to_meters(p, depth);

        // Pass the image to the SLAM system
        SLAM.TrackRGBD(imRGB,imD,tframe);
    }

    // Stop all threads
    SLAM.Shutdown();

    // Save camera trajectory
    SLAM.SaveTrajectoryTUM("CameraTrajectory.txt");
    SLAM.SaveKeyFrameTrajectoryTUM("KeyFrameTrajectory.txt");   

    return 0;
}
