#include "io_manager.hpp"



IOManager::IOManager(size_t workers_number)
    : io_context_{(int)workers_number}, workers_{workers_number}
    , signal_set_{io_context_, SIGTERM, SIGINT} {

    signal_set_.async_wait([this](auto, auto) { stop(); });
}

IOManager::~IOManager() {
    stop();
}

void IOManager::start() {
    spdlog::info("I/O manager starting...");
    for (auto& worker: workers_) {
        std::jthread thr([this] {work();});
        worker = std::move(thr);
    }
    spdlog::info("I/O manager started. workers number: {}", workers_.size());
    io_context_.run();
}

void IOManager::stop() {
    io_context_.stop();
    spdlog::info("I/O manager stopped");
}

net::io_context& IOManager::get_context() {
    return io_context_;
}

void IOManager::work() {
    spdlog::info("worker started: {}", std::hash<std::thread::id>{}(std::this_thread::get_id()));
    io_context_.run();
    spdlog::info("worker stopped: {}", std::hash<std::thread::id>{}(std::this_thread::get_id()));
}