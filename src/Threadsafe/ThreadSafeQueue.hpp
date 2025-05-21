//
// Created by alex on 19/05/25.
//

#ifndef THREADSAFEQUEUE_HPP
#define THREADSAFEQUEUE_HPP
#include <mutex>
#include <deque>
#include <shared_mutex>

template<typename T>
class ThreadSafeQueue {
private:
    std::deque<T> queue;
    std::shared_mutex mutex;
public:
    void push(T t) {
        std::unique_lock l(mutex);
        queue.push_back(std::move(t));
    }
    void pop() {
        std::unique_lock l(mutex);
        queue.pop_front();
    }
    ThreadSafeQueue(const ThreadSafeQueue&) = delete;
    ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;
    ThreadSafeQueue(ThreadSafeQueue&& other) = delete;
    ThreadSafeQueue& operator=(ThreadSafeQueue&& other) = delete;

    ThreadSafeQueue():queue{}, mutex{} {}

    bool empty() {
        std::shared_lock l{mutex};
        return queue.empty();
    }

    T front() {
        std::shared_lock l{mutex};
        return queue.front();
    }


    int size() {
        std::shared_lock l{mutex};
        return queue.size();
    }

    T front_and_pop() {
        std::unique_lock l(mutex);
        auto ret = queue.front();
        queue.pop_front();
        return ret;
    }
};
#endif //THREADSAFEQUEUE_HPP
