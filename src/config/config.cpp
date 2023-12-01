#include "config.hpp"

Config& Config::instance() {
    static Config instance;
    return instance;
}

void Config::initialize(std::string path) {
    std::ifstream f(path);
    json data = json::parse(f);
    f.close();
    if (!check_structure(data)) {
        throw std::runtime_error("Config structure is not valid");
    }
    address_ = data["address"];
    port_ = data["port"];
    workers_number_ = data["workers_number"];
    queue_length_ = data["queue_length"];
    operators_number_ = data["operators_number"];
    handling_min_time_ = data["handling_min_time"];
    handling_max_time_ = data["handling_max_time"];
    is_call_dupplicaton_handler_on_ = data["is_call_dupplicaton_handler_on"];
    spdlog::info("config is initialized");
}

std::string Config::get_address() const {
    return address_;
}

std::uint16_t Config::get_port() const {
    return port_;
}

size_t Config::get_workers_number() const {
    return workers_number_;
}

size_t Config::get_queue_length() const {
    return queue_length_;
}

size_t Config::get_operators_number() const {
    return operators_number_;
}

uint64_t Config::get_handling_min_time() const {
    return handling_min_time_;
}

uint64_t Config::get_handling_max_time() const {
    return handling_max_time_;
}

bool Config::is_call_dupplicaton_handler_on() const {
    return is_call_dupplicaton_handler_on_;
}

bool Config::check_structure(json& data) {
    return data.contains("address") &&
        data.contains("port") &&
        data.contains("workers_number") &&
        data.contains("queue_length") &&
        data.contains("operators_number") &&
        data.contains("handling_min_time") &&
        data.contains("handling_max_time") &&
        data.contains("is_call_dupplicaton_handler_on");

}