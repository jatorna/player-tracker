//
//  KalmanFilter.cpp
//  FootballVideoStats
//
//  Created by Javier Torro Nadal on 22/7/21.
//

#include "KalmanFilter.hpp"


KalmanFilter::KalmanFilter():
q_(10.0),
r_(1.0)
{
        //Do nothing
};


bool KalmanFilter::init(){

    kf_ = cv::KalmanFilter(4, 2);
    kf_.measurementMatrix = cv::Mat(2, 4, CV_32F, measurementMatrix_);
    kf_.transitionMatrix = cv::Mat(4, 4, CV_32F, transitionMatrix_);
    kf_.measurementNoiseCov = cv::Mat(2, 2, CV_32F, measurementNoiseCov_);
    kf_.processNoiseCov = cv::Mat(4, 4, CV_32F, processNoiseCov_);

    return true;

};


bool KalmanFilter::estimate(const cv::Point & meas){

    try{

        float meas_array[2] = {(float)meas.x, (float)meas.y};
        cv::Mat measurement(2, 1, CV_32F, meas_array);

        cv::Mat predicted = kf_.predict();

        if (meas_array[0] !=0 or meas_array[1] !=0){
            cv::Mat corrected = kf_.correct(measurement);
        }

        return true;

    }catch(...){
        BOOST_LOG_TRIVIAL(error) << "Error in kalman filter estimation";
        return false;
    };
};

KalmanFilter::~KalmanFilter(){};
