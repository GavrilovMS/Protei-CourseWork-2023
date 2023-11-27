#ifndef TS_QUEUE_H
#define TS_QUEUE_H

#include <iostream>
#include <condition_variable>
#include <mutex>
#include <vector>

template <typename T> 
class TSQueue {

protected:
    std::vector<T> buffer_;
    size_t start_, end_; 
    bool is_done_;
    mutable std::mutex mutex;
    std::condition_variable cond_cons_, cond_prod_;
    
    size_t size_, count_;

    bool is_full() const { return count_ == size_; }
    bool is_empty() const { return count_ == 0; }
    bool is_done() const { return is_done_; }

public:
    TSQueue(size_t size) : buffer_(size), start_(0), end_(0), size_(size), count_(0), is_done_(false) {

    }

    void push(T data) {
        std::unique_lock<std::mutex> lk{mutex};
        cond_prod_.wait(lk, [this] { return !is_full(); });

        count_++;
        buffer_[end_] = data;
        end_++;
        end_ %= buffer_.size();

        lk.unlock();
        cond_cons_.notify_one();
    }

    bool wait_and_pop(T &data) {

        std::unique_lock<std::mutex> lk{mutex};
        cond_cons_.wait(lk, [this] { return !is_empty() || is_done(); });
        if (is_empty()) {
            return false;
        }

        count_--;
        data = buffer_[start_];
        start_++;
        start_ %= buffer_.size();

        lk.unlock();
        cond_prod_.notify_one();
        return true;
    }

    void wake_and_done() {
        std::unique_lock<std::mutex> lk{mutex};
        is_done_ = true;
        lk.unlock();
        cond_cons_.notify_all();
    }

    bool is_full_protected() const {
        std::lock_guard<std::mutex> lk{mutex};
        return count_ == size_;
    }

    bool is_empty_and_done() const {
        std::unique_lock<std::mutex> lk{mutex};
        return is_empty() && is_done_;
    }

    size_t get_size() const {
        std::unique_lock<std::mutex> lk{mutex};
        return size_;
    }
};

#endif