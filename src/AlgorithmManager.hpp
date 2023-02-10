//
//  AlgorithmManager.hpp
//  FootballVideoStats
//
//  Created by Javier Torro Nadal on 22/7/21.
//

#ifndef AlgorithmManager_hpp
#define AlgorithmManager_hpp

//#include <stdio.h>
#include "KalmanFilter.hpp"
#include "ComputerVisionEngine.hpp"
#include "TrackingEngine.hpp"
#include "LowLevelFunctions.hpp"


class AlgorithmManager {
public:
    AlgorithmManager(const Config & config);
    ~AlgorithmManager();
    
    bool init();
    bool process();
    
private:

    Config config_;
    cv::VideoCapture vid_capture_;
    ComputerVisionEngine vision_engine_;
    TrackingEngine tracking_engine_;

};


#endif /* AlgorithmManager_hpp */
