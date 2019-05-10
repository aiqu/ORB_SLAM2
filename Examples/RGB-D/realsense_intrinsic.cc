#include <librealsense2/rs.hpp>
#include <iostream>

int main() {
	rs2::pipeline pipe;
    rs2::config cfg;
    cfg.enable_stream(RS2_STREAM_DEPTH, 640, 480, RS2_FORMAT_Z16, 30);
    cfg.enable_stream(RS2_STREAM_COLOR, 640, 480, RS2_FORMAT_BGR8, 30);

	rs2::pipeline_profile selection = pipe.start(cfg);
	std::cout << "Depth frame intrinsics --- \n";
	{
	auto depth_stream = selection.get_stream(RS2_STREAM_DEPTH)
				     .as<rs2::video_stream_profile>();
	auto resolution = std::make_pair(depth_stream.width(), depth_stream.height());
	auto i = depth_stream.get_intrinsics();
	auto principal_point = std::make_pair(i.ppx, i.ppy);
	auto focal_length = std::make_pair(i.fx, i.fy);
	rs2_distortion model = i.model;
	std::cout << resolution.first << "," << resolution.second << "\n"
		  << principal_point.first << "," << principal_point.second << "\n"
		  << focal_length.first << "," << focal_length.second << "\n"
		  << 0.015090852975845337 * focal_length.first << "\n"
		  << model << "\n";
	}
	std::cout << "Color frame intrinsics --- \n";
	{
	auto color_stream = selection.get_stream(RS2_STREAM_COLOR)
				     .as<rs2::video_stream_profile>();
	auto resolution = std::make_pair(color_stream.width(), color_stream.height());
	auto i = color_stream.get_intrinsics();
	auto principal_point = std::make_pair(i.ppx, i.ppy);
	auto focal_length = std::make_pair(i.fx, i.fy);
	rs2_distortion model = i.model;
	std::cout << resolution.first << "," << resolution.second << "\n"
		  << principal_point.first << "," << principal_point.second << "\n"
		  << focal_length.first << "," << focal_length.second << "\n"
		  << 0.015090852975845337 * focal_length.first << "\n"
		  << model << "\n";
	}
}
