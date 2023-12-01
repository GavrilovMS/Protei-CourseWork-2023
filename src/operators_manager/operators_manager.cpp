#include "operators_manager.hpp"

OperatorsManager::OperatorsManager(net::io_context& io_context, size_t operators_number, size_t queue_length) : 
    io_context_{io_context}, 
    calls_queue_{queue_length}, 
    operators_queue_{operators_number} {
    for (size_t i = 0; i < operators_number; i++) {
        Operator op;
        op.initialize();
        operators_queue_.push(op);
    }
}

OperatorsManager::~OperatorsManager() {
    calls_queue_.wake_and_done();
    operators_queue_.wake_and_done();
    is_working_ = false;
}

bool OperatorsManager::handle_call(Call call) {
    if (calls_queue_.is_full_protected()) {
        CDR::instance().write(call.get_call_date(), call.get_id(), call.get_phone_number(), CDR::Status::OVERLOAD);
        return false;
    } 

    size_t temp_index = calls_queue_.find_index(call.get_phone_number());
    if (temp_index != calls_queue_.get_size()) {
        if (Config::instance().is_call_dupplicaton_handler_on()) {
            call_dupplicaton_handler(call, temp_index);
            return true;
        } else {
            CDR::instance().write(call.get_call_date(), call.get_id(), call.get_phone_number(), CDR::Status::ALREADY_IN_QUEUE);
            return false;
        }
    }

    calls_queue_.push(call);
    return true;
}

void OperatorsManager::call_dupplicaton_handler(Call call, size_t index) {
    Call canceled_call = calls_queue_.get_call(index);
    calls_queue_.erase(index);
    CDR::instance().write(canceled_call.get_call_date(), canceled_call.get_id(), canceled_call.get_phone_number(), CDR::Status::CALL_DUPLICATION);
    calls_queue_.push(call);
}

void OperatorsManager::start() {
    spdlog::info("operators manager starting...");
    std::jthread thr([this] {work();});
    working_thread_ = std::move(thr);
    is_working_ = true;
}

void OperatorsManager::stop() {
    calls_queue_.wake_and_done();
    operators_queue_.wake_and_done();
    is_working_ = false;
    spdlog::info("operators manager stoped");
}


void OperatorsManager::work() {
    spdlog::info("operators manager started. number of operators: {}. calls queue size: {}", operators_queue_.get_size(), calls_queue_.get_size());
    while (is_working_) {
        Call call; Operator op;
        bool operator_found = operators_queue_.wait_and_pop(op);
        bool call_found = calls_queue_.wait_and_pop(call);
        while(call.is_timeout()) {
            CDR::instance().write(call.get_call_date(), call.get_id(), call.get_phone_number(), CDR::Status::TIMEOUT);
            call_found = calls_queue_.wait_and_pop(call);
        }
        if (call_found && operator_found) {
            co_spawn(io_context_, work_with_call(op, call), net::detached);
        }
    }
}

net::awaitable<void> OperatorsManager::work_with_call(Operator op, Call call) {
    op.set_busy();

    spdlog::info("operator {} is working...", uuids::to_string(op.get_id()));

    auto executor = co_await net::this_coro::executor;
    net::deadline_timer timer(executor, posix_time::seconds(op.get_working_time()));
    co_await timer.async_wait(net::use_awaitable);
    
    op.set_free();

    spdlog::info("operator {} is free...", uuids::to_string(op.get_id()));

    CDR::instance().write(call.get_call_date(),call.get_id(), call.get_phone_number(), op.get_end_date(), CDR::Status::OK, op.get_begin_date(), op.get_id());

    operators_queue_.push(op);
    co_return;
}

