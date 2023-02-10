//
//  Types.h
//  FootballVideoStats
//
//  Created by Javier Torro Nadal on 22/7/21.
//

#ifndef Types_h
#define Types_h

#include <string>

struct Config{
    
    std::string input_path;
    std::string output_path;
    bool use_kalman_filter;
};

typedef float Position[2];

#endif /* Types_h */
