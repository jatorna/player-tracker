//
//  AlgorithmManager.cpp
//  FootballVideoStats
//
//  Created by Javier Torro Nadal on 22/7/21.
//

#include "AlgorithmManager.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

AlgorithmManager::AlgorithmManager(const Config &config) :
		config_(config), vision_engine_(config), tracking_engine_(config) {

	//Do nothing
}
;

bool AlgorithmManager::init() {

	BOOST_LOG_TRIVIAL(info)
	<< "Initializing algorithms";

	bool valid = true;
	//TODO: Video capture does not work
	vid_capture_ = cv::VideoCapture(config_.input_path);
	valid = valid && vid_capture_.isOpened();
	valid = valid && vision_engine_.init();
	valid = valid && tracking_engine_.init();

	return valid;
}
;

bool AlgorithmManager::process() {

	BOOST_LOG_TRIVIAL(info)
	<< "Launching process";

	bool valid = true;

	while (vid_capture_.isOpened()) {

		cv::Mat frame;
		if (!vid_capture_.read(frame)) {
			BOOST_LOG_TRIVIAL(info)
			<< "Video is finished";
			break;
		}

		valid = valid && vision_engine_.process(frame);
		valid = valid
				&& tracking_engine_.UpdatePositions(
						vision_engine_.getBallPosition(),
						vision_engine_.getPlayersPosition());
		valid = valid && tracking_engine_.process(frame);

		if (!valid) {
			break;
		}

//        cv::imshow("Frame", frame);
//        cv::waitKey(20);

	}

	vid_capture_.release();
	cv::destroyAllWindows();

	return valid;
}
;

AlgorithmManager::~AlgorithmManager() {
}
;
