#ifndef OPERATOR_H
#define OPERATOR_H

#include <iostream>

#include <boost/uuid/uuid.hpp>            
#include <boost/uuid/uuid_generators.hpp> 
#include <boost/uuid/uuid_io.hpp>      
       
#include <boost/date_time/posix_time/posix_time.hpp>

#include <spdlog/spdlog.h>

#include "config.hpp"
#include "call.hpp"


namespace uuids = boost::uuids;
namespace posix_time = boost::posix_time;

class Operator {
private:
    uuids::uuid uuid_;
    bool is_busy_;

    uint64_t working_time_;
    posix_time::ptime begin_date_;
    posix_time::ptime end_date_;

    void generate_working_time();

public:
    Operator();
    ~Operator();

    uuids::uuid get_id() const;
    posix_time::ptime  get_begin_date() const;
    posix_time::ptime get_end_date() const;
    uint64_t get_working_time() const;
    bool is_busy();

    void set_busy();
    void set_free();

};
#endif