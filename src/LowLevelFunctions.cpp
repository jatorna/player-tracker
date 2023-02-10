//
//  low_level_functions.cpp
//  FootballVideoStats
//
//  Created by Javier Torro Nadal on 21/7/21.
//

#include "LowLevelFunctions.hpp"


bool init_logging(const std::string & log_level_str){
    
    logging::trivial::severity_level log_level = logging::trivial::info;
    
    if (log_level_str == "trace") {
        log_level = logging::trivial::trace;
    }else if (log_level_str == "debug") {
        log_level = logging::trivial::debug;
    }else if (log_level_str == "info") {
        log_level = logging::trivial::info;
    }else{
        return false;
    }
    
    logging::core::get()->set_filter(logging::trivial::severity >= log_level);
    logging::add_console_log(std::cout, logging::keywords::format = "[%TimeStamp%][%Severity%] %Message%");
//    logging::add_console_log(std::cout, logging::keywords::format = expr::stream<< expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d, %H:%M:%S.%f")<< " --> " << expr::message );
    //TODO: the next line causes a segmentation fault
    //	logging::add_common_attributes();
    return true;

}
