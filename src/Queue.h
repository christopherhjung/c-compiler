//
// Created by Christopher Jung on 13.11.20.
//

#pragma once


#include <mutex>
#include <condition_variable>
#include <deque>

template <class T>
class Queue
{
private:
    std::mutex mutex;
    std::condition_variable condition;
    std::deque<T*> queue;
    bool closed;
public:
    void offer(T* value) {
        {
            std::unique_lock<std::mutex> lock(this->mutex);
            queue.push_front(value);
        }
        this->condition.notify_one();
    }

    void close(){
        offer(nullptr);
    }

    T* take() {
        if(closed){
            throw std::exception();
        }

        std::unique_lock<std::mutex> lock(this->mutex);
        this->condition.wait(lock, [=]{ return !this->queue.empty(); });
        T* copy(std::move(this->queue.back()));
        this->queue.pop_back();
        if(copy == nullptr){
            closed = true;
        }
        return copy;
    }
};