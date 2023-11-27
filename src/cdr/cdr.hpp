#ifndef CDR_H
#define CDR_H

#include <fstream>
#include <string>
#include <iostream>
#include <mutex>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/uuid/uuid.hpp>   
#include <boost/uuid/uuid_io.hpp>

#include <spdlog/spdlog.h>

namespace posix_time = boost::posix_time;
namespace uuids = boost::uuids;

class CDR {
private:
   

    std::fstream file_;
    std::mutex mutex_;
public:
    CDR() = default;
    ~CDR();

    CDR(const CDR&) = delete;
    CDR(CDR&&) = delete;
    CDR& operator=(CDR const&) = delete;
    CDR& operator=(CDR&&) = delete;

    static CDR& instance();

    void initialize(std::string path);
    
    enum Status{OK, ALREADY_IN_QUEUE, CALL_DUPLICATION, TIMEOUT, OVERLOAD};
    std::string status_to_string(Status status);
    void write(const posix_time::ptime call_begin_date, const uuids::uuid call_id, const std::string phone_number,\
                const posix_time::ptime call_end_date, const Status status, const posix_time::ptime answer_date, const uuids::uuid operator_id);
    void write(const posix_time::ptime call_begin_date, const uuids::uuid call_id, const std::string phone_number, const Status status);
};

#endif 
