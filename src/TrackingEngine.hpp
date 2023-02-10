//
//  TrackingEngine.hpp
//  FootballVideoStats
//
//  Created by Javier Torro Nadal on 28/7/21.
//

#ifndef TrackingEngine_hpp
#define TrackingEngine_hpp

#include <stdio.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video/video.hpp>
#include "KalmanFilter.hpp"
#include "LowLevelFunctions.hpp"
#include "Types.h"


class TrackingEngine {
public:
    TrackingEngine(const Config & config);
    ~TrackingEngine();
    
    bool init();
    bool process(cv::Mat & frame);
    bool UpdatePositions(const cv::Point & ball, const std::vector<cv::Point> & players);


private:
    
    bool detectPlayers(cv::Mat & frame);
    double getDistance(const cv::Point & pt1, const cv::Point & pt2);
    
    cv::Point ball_;
    std::vector<cv::Point> players_;
    std::vector<std::pair<bool, cv::Point>> players_detected_;
    std::vector<cv::Point> players_old_;
    KalmanFilter ball_kf_;
    std::vector<KalmanFilter> players_kf_;
    Config config_;
};

#endif /* TrackingEngine_hpp */
