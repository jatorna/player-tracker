//
//  ComputerVisionEngine.cpp
//  FootballVideoStats
//
//  Created by Javier Torro Nadal on 27/7/21.
//

#include "ComputerVisionEngine.hpp"

ComputerVisionEngine::ComputerVisionEngine(const Config &config) :
		config_(config) {
	//Do nothing
}
;

bool ComputerVisionEngine::init() {

	//TODO: initizalize constants

	return true;
}
;

bool ComputerVisionEngine::process(cv::Mat &frame) {

	BOOST_LOG_TRIVIAL(info)
	<< "Computer vision process started";

	bool valid = true;

	cv::Mat bgExtract, bgMask, SobelG, imgFinal;
	valid = valid && RGBcondition(frame, bgExtract, bgMask);
	valid = valid && SobelGradient(frame, 40, SobelG);

	if (valid) {
		cv::bitwise_and(bgExtract + SobelG, bgMask, imgFinal);
		cv::Mat kernel(cv::Size(2, 2), CV_8U, cv::Scalar(1));
		cv::morphologyEx(imgFinal, imgFinal, cv::MORPH_CLOSE, kernel);
	}

	valid = valid && HoughTransform(imgFinal, 6, CV_PI / 60, 500, 80, 15);
	valid = valid && Tagged(imgFinal, frame);

	return valid;
}
;

bool ComputerVisionEngine::RGBcondition(const cv::Mat &img,
		cv::Mat &bgExtract_out, cv::Mat &bgMask_out) {

	cv::Mat BGRchannels[3];

	cv::split(img, BGRchannels);

	cv::Mat bgExtract1 = BGRchannels[1] > BGRchannels[2];
	cv::Mat bgExtract2 = BGRchannels[2] > BGRchannels[0];

	bgExtract1.convertTo(bgExtract1, CV_8U);
	bgExtract2.convertTo(bgExtract2, CV_8U);

	cv::Mat bgExtract;
	cv::bitwise_and(bgExtract1, bgExtract2, bgExtract);

	bgExtract.convertTo(bgExtract, CV_8U);
	bgExtract *= 255;

	cv::Mat bgMask(cv::Size(img.size[1], img.size[0]), CV_8U, cv::Scalar(0));

	cv::Mat hierarchy;
	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(bgExtract, contours, hierarchy, cv::RETR_EXTERNAL,
			cv::CHAIN_APPROX_NONE);

	cv::drawContours(bgMask, contours, getMaxAreaContourId(contours),
			cv::Scalar(255), -1);

	cv::Mat bgExtract_final;
	cv::bitwise_not(bgExtract, bgExtract_final);

	bgExtract_out = bgExtract_final;
	bgMask_out = bgMask;

	BOOST_LOG_TRIVIAL(info)
	<< "Aplying RGB condition";

	return true;
}
;

bool ComputerVisionEngine::ReadMediaFile(cv::Mat &img) {

	cv::VideoCapture vid_capture(config_.input_path);

	if (!vid_capture.isOpened()) {
		BOOST_LOG_TRIVIAL(error)
		<< "Error reading video file";
		return false;
	}

	return vid_capture.read(img);
}
;

int ComputerVisionEngine::getMaxAreaContourId(
		std::vector<std::vector<cv::Point>> contours) {
	double maxArea = 0;
	int maxAreaContourId = -1;
	for (long unsigned int j = 0; j < contours.size(); j++) {
		double newArea = cv::contourArea(contours.at(j));
		if (newArea > maxArea) {
			maxArea = newArea;
			maxAreaContourId = j;
		} // End if
	} // End for
	return maxAreaContourId;
}
;

bool ComputerVisionEngine::SobelGradient(const cv::Mat &img,
		const int &threshold, cv::Mat &SobelG) {

	cv::Mat gray, gray_gaus, dX, dY, mag, angle;
	cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
	cv::GaussianBlur(gray, gray_gaus, cv::Size(5, 5), 0.0);
	cv::Sobel(gray_gaus, dX, CV_32F, 1, 0, 3);
	cv::Sobel(gray_gaus, dY, CV_32F, 0, 1, 3);
	cv::cartToPolar(dX, dY, mag, angle, true);
	cv::threshold(mag, SobelG, threshold, 255, cv::THRESH_BINARY);

	SobelG.convertTo(SobelG, CV_8U);

	BOOST_LOG_TRIVIAL(info)
	<< "Sobel gradient perform";
	return true;
}

bool ComputerVisionEngine::HoughTransform(cv::Mat &img, double rho,
		double theta, int threshold, double minLenght, double maxGap) {

	cv::Mat lines;
	cv::HoughLinesP(img, lines, rho, theta, threshold, minLenght, maxGap);

	for (int i = 0; i < lines.rows; i++) {

		int x1 = lines.at<int>(i * lines.cols);
		int x2 = lines.at<int>(i * lines.cols + 1);
		int y1 = lines.at<int>(i * lines.cols + 2);
		int y2 = lines.at<int>(i * lines.cols + 3);

		cv::Point start(x1, x2);
		cv::Point end(y1, y2);

		drawDiscLine(img, start, end, 7);
	}

	BOOST_LOG_TRIVIAL(info)
	<< "Hough transform perform";

	return true;
}

bool ComputerVisionEngine::drawDiscLine(cv::Mat &img, cv::Point start,
		cv::Point end, const int &thickness) {

	double paso_x = (double) abs(start.x - end.x) / 16;
	double paso_y = (double) (end.y - start.y) / 16;

	int arr[4] = { 0, 4, 8, 12 };

	for (auto i : arr) {

		cv::Point pt1((int) start.x + i * paso_x, (int) start.y + i * paso_y);
		cv::Point pt2((int) start.x + (i + 1) * paso_x,
				(int) start.y + (i + 1) * paso_y);
		cv::line(img, pt1, pt2, cv::Scalar(0, 0, 0), thickness);
	}

	return true;
}

bool ComputerVisionEngine::Tagged(const cv::Mat &img, cv::Mat &frame) {

	players_.clear();
	cv::Mat labels, stats, centroids;
	cv::connectedComponentsWithStats(img, labels, stats, centroids);

	for (int i = 0; i < stats.rows; i++) {

		int area = stats.at<int>(i * stats.cols + cv::CC_STAT_AREA);
		int height = stats.at<int>(i * stats.cols + cv::CC_STAT_HEIGHT);
		int width = stats.at<int>(i * stats.cols + cv::CC_STAT_WIDTH);

		double density = (double) area / (height * width);

		// Players
		if ((area > 500) && (area < 2000) && ((double) height / width > 1)
				&& ((double) height / width < 3) && (density > 0.3)
				&& (density < 0.9)) {
			players_.push_back(
					TagElement(i, area, height, width, centroids, stats,
							frame));
		}

		// Ball
		if ((area > 100) && (area < 300) && ((double) height / width > 0.5)
				&& ((double) height / width <= 1.3) && (density > 0.7)
				&& (density < 0.9)) {
			ball_ = TagElement(i, area, height, width, centroids, stats, frame);

			std::cout << "Ball position in tagged " << ball_ << std::endl;
		}
	};

	BOOST_LOG_TRIVIAL(info)
	<< "Tagged performed";

	return true;
}
;

cv::Point ComputerVisionEngine::TagElement(const int &index, const int &area,
		const int &height, const int &width, const cv::Mat &centroids,
		const cv::Mat &stats, cv::Mat &frame) {
	int x_bb = stats.at<int>(index * stats.cols + cv::CC_STAT_LEFT);
	int y_bb = stats.at<int>(index * stats.cols + cv::CC_STAT_TOP);

	cv::Point pt1(x_bb, y_bb), pt2(x_bb + width, y_bb + height), center(
			(int) centroids.at<double>(index * centroids.cols + 0),
			(int) centroids.at<double>(index * centroids.cols + 1));

	cv::rectangle(frame, pt1, pt2, cv::Scalar(0, 0, 255), 2);
	cv::circle(frame, center, 3, cv::Scalar(0, 255, 0), -1);

	return center;
}
;

cv::Point ComputerVisionEngine::getBallPosition() const {
	return ball_;
}
;

std::vector<cv::Point> ComputerVisionEngine::getPlayersPosition() const {
	return players_;
}
;

ComputerVisionEngine::~ComputerVisionEngine() {
}
;
