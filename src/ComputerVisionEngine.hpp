//
//  ComputerVisionEngine.hpp
//  FootballVideoStats
//
//  Created by Javier Torro Nadal on 27/7/21.
//

#ifndef ComputerVisionEngine_hpp
#define ComputerVisionEngine_hpp

#include <stdio.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "LowLevelFunctions.hpp"
#include "Types.h"


class ComputerVisionEngine {
public:
    ComputerVisionEngine(const Config & config);
    ~ComputerVisionEngine();
    
    bool init();
    bool process(cv::Mat & frame);
    cv::Point getBallPosition() const;
    std::vector<cv::Point> getPlayersPosition() const;

private:
    
    bool RGBcondition(const cv::Mat & img, cv::Mat & bgExtract_out, cv::Mat & bgMask_out);
    bool SobelGradient(const cv::Mat & img, const int & threshold, cv::Mat & SobelG);
    bool ReadMediaFile(cv::Mat & img);
    int getMaxAreaContourId(std::vector <std::vector<cv::Point>> contours);
    bool HoughTransform(cv::Mat & img, double rho, double theta, int threshold, double minLenght, double maxGap);
    bool drawDiscLine(cv::Mat & img, cv::Point start, cv::Point end, const int & thickness);
    bool Tagged(const cv::Mat & img, cv::Mat & frame);
    cv::Point TagElement(const int & index, const int & area, const int & height, const int & width, const cv::Mat & centroids, const cv::Mat & stats, cv::Mat & frame);
   

    cv::Point ball_;
    std::vector<cv::Point> players_;
    Config config_;
    
};

#endif /* ComputerVisionEngine_hpp */
