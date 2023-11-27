#include "calls_queue.hpp"


CallsQueue::CallsQueue(size_t queue_length) : TSQueue{queue_length}  {
    
}

CallsQueue::~CallsQueue() {

}

size_t CallsQueue::find_index(std::string phone_number) {
    std::lock_guard<std::mutex> lk{mutex};
    for (size_t i = start_; i != end_; i = (i+1) % buffer_.size()) {
        if (buffer_[i].get_phone_number() == phone_number) {
            return i;
        }   
    }
    return buffer_.size();
}

Call CallsQueue::get_call(size_t index) {
    std::lock_guard<std::mutex> lk{mutex};
    return buffer_[index];
}

void CallsQueue::erase(size_t index) {
    std::lock_guard<std::mutex> lk{mutex};
    end_--;
    end_ %= buffer_.size();
    count_--;
    
    for (size_t i = index; i != end_; i = (i + 1) % buffer_.size()) {
        buffer_[i] = buffer_[(i+1)%buffer_.size()];
    }
    
}