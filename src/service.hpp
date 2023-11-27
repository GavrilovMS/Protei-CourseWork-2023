#ifndef SERVICE_H
#define SERVICE_H

#include "io_manager.hpp"
#include "controller.hpp"
#include "operators_manager.hpp"

#include "spdlog/spdlog.h"

class Service{
private:
    IOManager io_manager_;
    Controller controller_;
    OperatorsManager operators_manager_;
public:

    Service(size_t workers_number, size_t operators_number, size_t queue_length);
    ~Service() = default;
    void run(std::uint16_t port, std::string address);
};
#endif 

