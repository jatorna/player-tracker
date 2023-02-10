//
//  low_level_functions.hpp
//  FootballVideoStats
//
//  Created by Javier Torro Nadal on 21/7/21.
//

#define BOOST_LOG_DYN_LINK 1

#ifndef low_level_functions_hpp
#define low_level_functions_hpp

#include <stdio.h>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

namespace logging = boost::log;
namespace expr = logging::expressions;

bool init_logging(const std::string & log_level_str);

#endif /* low_level_functions_hpp */
