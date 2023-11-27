#include "service.hpp"

Service::Service(size_t workers_number, size_t operators_number, size_t queue_length)
    : io_manager_{workers_number}, 
    operators_manager_ {io_manager_.get_context(), operators_number, queue_length }, 
    controller_{io_manager_.get_context(), operators_manager_} {
        spdlog::info("service is initialized");
    }


void Service::run(std::uint16_t port, std::string address) {
    spdlog::info("service running...");
    std::exception_ptr saved_ex;
    try {
        operators_manager_.start();
        controller_.start(address, port);
        io_manager_.start();
    }
    catch (const std::exception& ex) {
        saved_ex = std::current_exception();
        spdlog::critical("not services were started: {}", ex.what());
        std::rethrow_exception(saved_ex);
    }
    spdlog::info("service is stopping...");
    try {
        operators_manager_.stop();
        controller_.stop();
    }
    catch (const std::exception& ex) {
        saved_ex = std::current_exception();
        spdlog::critical("not services were stopped: {}", ex.what());
    }
    spdlog::info("service finished successfully");
}

