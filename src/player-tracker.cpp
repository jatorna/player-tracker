//============================================================================
// Name        : player-tracker.cpp
// Author      : jatorna
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

#include <iostream>
#include <boost/program_options.hpp>
#include <stdlib.h>
#include "LowLevelFunctions.hpp"
#include "AlgorithmManager.hpp"


int main(int argc, const char * argv[]) {

    namespace po = boost::program_options;
    po::variables_map vm;
    po::options_description generic("Generic options");
    po::options_description conf("Configuration");
    po::options_description visible("Allowed options");

    try
    {
        generic.add_options()
        ("version,v", "print version")
        ("help,h", "produce help message");

        conf.add_options()
        ("input_path,i", po::value< std::string >()->required(), "specifies input path")
        ("output_path,o", po::value< std::string >()->required(), "specifies output path")
        ("use_kalman_filter,k", "use kalman filter flag")
        ("log_level,g", po::value< std::string >()->default_value("info"), "specifies log level. info/debug/trace");

        visible.add(generic).add(conf);

        po::store(po::parse_command_line(argc, argv, visible), vm);

        if (vm.count("help")) {
            std::cout << visible << "\n";
            return 1;
        }

        if (vm.count("version")) {
            std::cout << "Football Video Stats v1.0" << "\n";
            return 1;
        }

        po::notify(vm);
    }
    catch(std::exception& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        std::cout<<visible<<std::endl;
        exit (EXIT_FAILURE);
    }
    catch(...)
    {
        std::cerr << "Unknown error!" << "\n";
        std::cout<<visible<<std::endl;
        exit (EXIT_FAILURE);
    }

    if (!init_logging(vm["log_level"].as<std::string>())){
        std::cout<<"Log level not valid\n";
        std::cout<<visible<<std::endl;
        exit (EXIT_FAILURE);
    }

    BOOST_LOG_TRIVIAL(info) << "### Program started ###";

    Config config;
    config.input_path = vm["input_path"].as<std::string>();
    config.output_path = vm["output_path"].as<std::string>();
    config.use_kalman_filter = vm.count("use_kalman_filter");

    AlgorithmManager algorithm_mgr = AlgorithmManager(config);

    if (!algorithm_mgr.init()){
        BOOST_LOG_TRIVIAL(fatal) << "Error initializing algorithms. Exiting with error";
        exit (EXIT_FAILURE);
    }

    if (!algorithm_mgr.process()){
        BOOST_LOG_TRIVIAL(fatal) << "Error processing algorithms. Exiting with error";
        exit (EXIT_FAILURE);
    }else{
        BOOST_LOG_TRIVIAL(info) << "Process finished correctly";
    }

    BOOST_LOG_TRIVIAL(info) << "### Program finished ###";

    return 0;
}
