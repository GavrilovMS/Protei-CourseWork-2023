#ifndef OPERATORS_MANAGER_H
#define OPERATORS_MANAGER_H

#include <thread>

#include <boost/asio/io_context.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>

#include <spdlog/spdlog.h>

#include "operator.hpp"
#include "calls_queue.hpp"
#include "ts_queue.hpp"
#include "call.hpp"
#include "cdr.hpp"
#include "config.hpp"

#include <gtest/gtest.h>

namespace net = boost::asio;
namespace posix_time = boost::posix_time;

class OperatorsManager{
private:

    CallsQueue calls_queue_;
    TSQueue<Operator> operators_queue_;
    
    size_t operators_number_;
    net::io_context& io_context_;
    std::jthread working_thread_;

    bool is_working_;
    void work();
    net::awaitable<void> work_with_call(Operator op, Call call); 
    void call_dupplicaton_handler(Call call, size_t index);
public:
    
    OperatorsManager(net::io_context& io_context, size_t operators_number, size_t queue_length);
    ~OperatorsManager();

    bool handle_call(Call call);
    void start();
    void stop();

};


#endif