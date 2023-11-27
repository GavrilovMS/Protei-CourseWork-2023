#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <fstream>
#include <exception>

#include <nlohmann/json.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <spdlog/spdlog.h>

using json = nlohmann::json;
namespace posix_time = boost::posix_time;

class Config {
private:
    std::string address_;
    std::uint16_t port_;
    size_t workers_number_;
    size_t queue_length_; 
    size_t operators_number_;
    uint64_t handling_min_time_;
    uint64_t handling_max_time_;
    bool is_repeat_request_handler_on_;

    bool check_structure(json& data);

public:
    Config() = default;
    ~Config() = default;

    Config(const Config&) = delete;
    Config(Config&&) = delete;
    Config& operator=(Config const&) = delete;
    Config& operator=(Config&&) = delete;

    static Config& instance();

    void initialize(std::string path);

    std::string get_address() const;
    std::uint16_t get_port() const;
    size_t get_workers_number() const;
    size_t get_queue_length() const; 
    size_t get_operators_number() const;
    uint64_t get_handling_min_time() const;
    uint64_t get_handling_max_time() const;
    bool is_repeat_request_handler_on() const;
};

#endif