#include "operator.hpp"

Operator::Operator() : is_busy_{false} {
    uuid_ = uuids::random_generator()();
}

Operator::~Operator() {

}

uuids::uuid Operator::get_id() const {
    return uuid_;
}

posix_time::ptime Operator::get_begin_date() const {
    return begin_date_;
}

posix_time::ptime Operator::get_end_date() const {
    return end_date_;
}

uint64_t Operator::get_working_time() const {
    return working_time_;
}

bool Operator::is_busy() {
    return is_busy_;
}

void Operator::set_busy() {
    if (is_busy()) return;
    begin_date_ = posix_time::second_clock::local_time();
    generate_working_time();
    is_busy_ = true;
}

void Operator::set_free() {
    if (!is_busy()) return; 
    end_date_ = posix_time::second_clock::local_time();
    is_busy_ = false;
}

void Operator::generate_working_time() {
    posix_time::ptime now = posix_time::second_clock::local_time();
    uint64_t min = Config::instance().get_handling_min_time();
    uint64_t max = Config::instance().get_handling_max_time();
    uint64_t res = min + (rand() % (max - min));
    working_time_ = res;
}

