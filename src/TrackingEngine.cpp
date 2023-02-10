//
//  TrackingEngine.cpp
//  FootballVideoStats
//
//  Created by Javier Torro Nadal on 28/7/21.
//

#include "TrackingEngine.hpp"

TrackingEngine::TrackingEngine(const Config & config):
config_(config)
{
    // DO nothing
};


bool TrackingEngine::init(){
    
    bool valid = true;
    
    if (config_.use_kalman_filter){

        ball_kf_ = KalmanFilter();
        valid = valid && ball_kf_.init();

    }
    
    return valid;
}


bool TrackingEngine::process(cv::Mat & frame){
    
    BOOST_LOG_TRIVIAL(info) <<"Tracking process started";
    
    bool valid = true;
    
    if ((players_old_.size() == 0) || (players_.size() == 0)){
        BOOST_LOG_TRIVIAL(info) << "No players to track";
        players_old_ = players_;
        return valid;
    }else{
        BOOST_LOG_TRIVIAL(info) << "Players tracked: " <<players_.size();
    }
    
    valid = valid && detectPlayers(frame);
    
    if (valid && config_.use_kalman_filter){
        valid = valid && ball_kf_.estimate(ball_);
    }
    
    players_old_ = players_;
    
    return valid;
}


bool TrackingEngine::UpdatePositions(const cv::Point & ball, const std::vector<cv::Point> & players){
    
    bool valid = true;
    
    ball_ = ball;
    players_ = players;

    return valid;
}


double TrackingEngine::getDistance(const cv::Point & pt1, const cv::Point & pt2){
    return sqrt((pt1.x-pt2.x)*(pt1.x-pt2.x)+(pt1.y-pt2.y)*(pt1.y-pt2.y));
}


bool TrackingEngine::detectPlayers(cv::Mat & frame){
    
    players_detected_.clear();
    players_detected_.resize(players_old_.size());
    
    for (auto player : players_){
        
        double min_distance = 10000;
        bool change = false;
        int index = 0;
        
        int j = 0;
        for (auto player_old : players_old_){
         
            double distance = getDistance(player, player_old);
            
            if ((distance < min_distance) && (distance < 50) && !players_detected_[j].first){
                min_distance = distance;
                index = j;
                change = true;
            }
            j++;
        }
        
        if (change){
            players_detected_[index].first = true;
            players_detected_[index].second = player;
        }else{
            players_detected_.push_back(std::pair<bool, cv::Point>(true, player));
        }
    }
    
    // TODO put label in playes. Maybe with IA
//    int player_number = 1;
//    for (auto player : players_detected_){
//        if (player.first){
//            cv::putText(frame, "Player " + std::to_string(player_number), player.second, cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0,0,255));
//        }
//        player_number ++;
//    }
    
    return true;
}


TrackingEngine::~TrackingEngine(){}
