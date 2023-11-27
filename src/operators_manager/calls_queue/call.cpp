#include "call.hpp"

Call::Call(std::string phone_number) : phone_number_(phone_number) {
    uuid_ = uuids::random_generator()();
    call_date_ = posix_time::second_clock::local_time();
}

uuids::uuid Call::get_id() const {
    return uuid_;
} 

posix_time::ptime Call::get_call_date() const {
    return call_date_;
}

std::string Call::get_phone_number() const {
    return phone_number_;
}

bool Call::is_timeout() const {
    posix_time::ptime now = posix_time::second_clock::local_time();
    posix_time::time_duration max_duration(posix_time::seconds(Config::instance().get_handling_max_time()));
    return (now - call_date_) > max_duration;
}