//
//  KalmanFilter.hpp
//  FootballVideoStats
//
//  Created by Javier Torro Nadal on 22/7/21.
//

#ifndef KalmanFilter_hpp
#define KalmanFilter_hpp

#include <iostream>
#include <stdio.h>
#include "Types.h"
#include "LowLevelFunctions.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/video/video.hpp>


class KalmanFilter {
public:

    KalmanFilter();
   ~KalmanFilter();

    bool init();
    bool estimate(const cv::Point & meas);


private:

    cv::KalmanFilter kf_;
    float q_;
    float r_;
    float measurementMatrix_[2][4] = {{1,0,0,0},{0,1,0,0}};
    float transitionMatrix_[4][4] = {{1,0,1,0},{0,1,0,1},{0,0,1,0},{0,0,0,1}};
    float measurementNoiseCov_[2][2] = {{r_*r_,0},{0,r_*r_}};
    float processNoiseCov_[4][4] = {{q_*q_,0,0,0},{0,q_*q_,0,0},{0,0,q_*q_,0},{0,0,0,q_*q_}};


};

#endif /* KalmanFilter_hpp */
