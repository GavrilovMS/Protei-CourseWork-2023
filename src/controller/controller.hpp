#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <iostream>
#include <memory>

#include <boost/asio/io_context.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio.hpp>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

#include <spdlog/spdlog.h>

#include "operators_manager.hpp"
#include "call.hpp"

namespace net = boost::asio;
using tcp = net::ip::tcp;
namespace beast = boost::beast;
namespace http = beast::http;

class Controller : public std::enable_shared_from_this<Controller>{
private:

    net::io_context& io_context_;
    OperatorsManager& operators_manager_;

    net::awaitable<void> listen(std::string address, std::uint16_t port);
    net::awaitable<void> handle_accept(tcp::socket socket);

    http::response<http::dynamic_body> handle_request(http::request<http::dynamic_body>& request);
    void create_response(http::response<http::dynamic_body>& response, http::request<http::dynamic_body>& request);

public:

    Controller(net::io_context& io_context, OperatorsManager& operators_manager);
    ~Controller() = default;

    void start(std::string address, std::uint16_t port);
    void stop();
};


#endif