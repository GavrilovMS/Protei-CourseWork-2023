#ifndef CALL_H
#define CALL_H

#include <iostream>
#include <string>

#include <boost/date_time/posix_time/posix_time.hpp>

#include <boost/uuid/uuid.hpp>            
#include <boost/uuid/uuid_generators.hpp> 

#include "config.hpp"

namespace posix_time = boost::posix_time;
namespace uuids = boost::uuids;

class Call {
private:
    uuids::uuid uuid_;
    posix_time::ptime call_date_;
    std::string phone_number_;
public:
    Call() = default;
    Call(std::string phone_number);
    ~Call() = default;

    uuids::uuid  get_id() const;
    posix_time::ptime get_call_date() const;
    std::string get_phone_number() const;
    bool is_timeout() const;
};
#endif