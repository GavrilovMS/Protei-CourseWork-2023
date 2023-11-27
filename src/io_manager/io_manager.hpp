#ifndef IO_MANAGER_H
#define IO_MANAGER_H

#include <vector>
#include <thread>

#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>

#include <spdlog/spdlog.h>

namespace net = boost::asio;

class IOManager {
private:
    net::io_context io_context_;
    net::signal_set signal_set_;
    std::vector<std::jthread> workers_;

    void work();
public:
    explicit IOManager(size_t workers_number);
    ~IOManager();

    void start();
    void stop();

    net::io_context& get_context();
};

#endif