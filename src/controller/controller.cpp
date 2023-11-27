#include "controller.hpp"



Controller::Controller(net::io_context& io_context, OperatorsManager& operators_manager) : io_context_{io_context}, operators_manager_{operators_manager} {

}


void Controller::start(std::string address, std::uint16_t port) {
    spdlog::info("controller starting...");
    net::co_spawn(io_context_, listen(address, port), net::detached);
}

void Controller::stop() {
    spdlog::info("controller stopped");
}

net::awaitable<void> Controller::listen(std::string address, std::uint16_t port) {
    spdlog::info("controller is listening: {}/{}", address, port);
    auto executor = co_await net::this_coro::executor;
    tcp::acceptor acceptor{io_context_, {net::ip::make_address(address), port}};
    tcp::socket socket{io_context_};

    for (;;) {
        co_await acceptor.async_accept(socket, net::use_awaitable);
        net::co_spawn(executor, handle_accept(std::move(socket)), net::detached);
        spdlog::info("acception is being handled...");
    }
}

net::awaitable<void> Controller::handle_accept(tcp::socket socket) {
    try {
        beast::flat_buffer buffer{8192};
        http::request<http::dynamic_body> request;


        co_await http::async_read(socket, buffer, request, net::use_awaitable);
        
        http::response<http::dynamic_body> response = handle_request(request);
        
        co_await http::async_write(socket, response, net::use_awaitable);
        spdlog::info("acception is handled");
    }
    catch (const std::exception& ex) {
        
    }
}

http::response<http::dynamic_body> Controller::handle_request(http::request<http::dynamic_body>& request) {
    http::response<http::dynamic_body> response;
    response.version(request.version());
    response.keep_alive(false);
    switch(request.method()) {
    case http::verb::post:
        create_response(response, request);
        break;
    default:
        response.result(http::status::bad_request);
        response.set(http::field::content_type, "text/plain");
        beast::ostream(response.body())
            << "Invalid request-method '"
            << std::string(request.method_string())
            << "'";
        break;
    }
    spdlog::info("request handled");
    return response;
}

   
void Controller::create_response(http::response<http::dynamic_body>& response, http::request<http::dynamic_body>& request) {
    if(request.target() == "/call")
    {
        response.set(http::field::content_type, "text");

        std::string phone_number = beast::buffers_to_string(request.body().data());
        Call call(phone_number);
        bool succ = operators_manager_.handle_call(call);

        if (succ) {
            response.result(http::status::ok);
            beast::ostream(response.body()) << "CallId: " << call.get_id() << '\n';
            spdlog::info("response created. call id: {}", uuids::to_string(call.get_id()));
        } else {
            response.result(http::status::service_unavailable);
            spdlog::warn("response created. handling error.");
        };
        response.set(http::field::server, "Beast");
    }
    else
    {
        response.result(http::status::not_found);
        response.set(http::field::content_type, "text/plain");
        beast::ostream(response.body()) << "File not found\r\n";
        spdlog::error("response created. file is not found.");
    }
}
