#include "cdr.hpp"

CDR::~CDR() {
    file_.close();
    spdlog::info("CDR is closed");
}

CDR& CDR::instance() {
    static CDR instance;
    return instance;
}

void CDR::initialize(std::string path) {
    file_.open(path, std::fstream::in  | std::fstream::app);
    if (!file_.is_open()) {
        spdlog::error("CDR initialization error");
    }
    spdlog::info("CDR is initialized"); 
}


std::string CDR::status_to_string(Status status) {
    switch (status) {
        case OK: {
            return "OK";
        }
        case ALREADY_IN_QUEUE: {
            return "ALREADY IN QUEUE";
        }
        case CALL_DUPLICATION: {
            return "CALL DUPLICATION";
        }
        case TIMEOUT: {
            return "TIMEOUT";
        }
        case OVERLOAD: {
            return "OVERLOAD";
        }
        default: {
            return "unknown status";
        }
    }
}
void CDR::write(const posix_time::ptime call_begin_date, const uuids::uuid call_id, const std::string phone_number,\
                const posix_time::ptime call_end_date, const Status status, \
                const posix_time::ptime answer_date, const uuids::uuid operator_id) {
    if (file_.is_open()) {
        std::string cb_date, c_id, ph_num, st, ce_date, ans_date, op_id, c_len;

        cb_date = posix_time::to_iso_extended_string(call_begin_date);
        c_id = uuids::to_string(call_id);
        ph_num = phone_number;
        st = status_to_string(status);
        ce_date = posix_time::to_iso_extended_string(call_end_date);
        ans_date  = posix_time::to_iso_extended_string(answer_date);
        op_id = uuids::to_string(operator_id);
        posix_time::time_duration duration = call_end_date - answer_date;
        c_len = posix_time::to_simple_string(duration);
        
        std::string str = cb_date + ';' + c_id + ';' + ph_num + ';' + ce_date + ';' + st + ';' + ans_date + ';' + op_id + ';' + c_len + '\n';
        
        std::lock_guard<std::mutex> lk{mutex_};
        file_ << str;
        file_.flush();
        spdlog::info("CDR note is written. status: {}", st);
    } else {
        spdlog::warn("CDR is not opened");
    }
}

void CDR::write(const posix_time::ptime call_begin_date, const uuids::uuid call_id, const std::string phone_number, const Status status) {
    if (file_.is_open()) {
        std::string cb_date, c_id, ph_num, st, ce_date, ans_date, op_id, c_len;

        cb_date = posix_time::to_iso_extended_string(call_begin_date);
        c_id = uuids::to_string(call_id);
        ph_num = phone_number;
        st = status_to_string(status);
        ce_date = "";
        ans_date  = "";
        op_id = "";
        c_len = "";

        std::string str = cb_date + ';' + c_id + ';' + ph_num + ';' + ce_date + ';' + st + ';' + ans_date + ';' + op_id + ';' + c_len + '\n';
        
        std::lock_guard<std::mutex> lk{mutex_};
        file_ << str;
        file_.flush();
        spdlog::info("CDR note is written. status: {}", st);
    } else {
        spdlog::warn("CDR is not opened");
    }
}