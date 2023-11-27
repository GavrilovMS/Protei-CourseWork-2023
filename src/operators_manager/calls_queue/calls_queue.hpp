#ifndef CALLS_QUEUE_H
#define CALLS_QUEUE_H

#include "ts_queue.hpp"
#include "call.hpp"

#include <string>

class CallsQueue : public TSQueue<Call>{
    
public:
    CallsQueue(size_t queue_length);
    ~CallsQueue();
    size_t find_index(std::string phone_number); 
    Call get_call(size_t index);
    void erase(size_t index);   
};

#endif