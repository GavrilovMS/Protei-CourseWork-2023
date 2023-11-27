#include <spdlog/spdlog.h>

#include "config.hpp"
#include "cdr.hpp"
#include "service.hpp"


int main(int argc, char** argv){
    try {
        Config::instance().initialize("/home/matvey/Projects/ProteiCourseWork/configs/config.json");
        CDR::instance().initialize("CDR.txt");

        Service service(Config::instance().get_workers_number(), Config::instance().get_operators_number(), Config::instance().get_queue_length());
        service.run(Config::instance().get_port(), Config::instance().get_address());
        
        return EXIT_SUCCESS;        
    } catch (const std::exception ex) {
        spdlog::critical("service error: {}", ex.what());
    }
    return EXIT_FAILURE;
}


