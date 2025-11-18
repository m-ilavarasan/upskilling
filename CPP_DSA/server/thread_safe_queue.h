#ifndef THREAD_SAFE_QUEUE_H
#define THREAD_SAFE_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <utility>

// Simple header-only thread-safe queue.
// Usage:
//   ThreadSafeQueue<int> q;
//   q.push(42);
//   auto v = q.wait_and_pop(); // returns std::shared_ptr<int>

template <typename T>
class ThreadSafeQueue
{
public:
    ThreadSafeQueue() = default;
    ThreadSafeQueue(const ThreadSafeQueue &) = delete;
    ThreadSafeQueue &operator=(const ThreadSafeQueue &) = delete;

    // Push by value (copy or move)
    void push(T value)
    {
        std::lock_guard<std::mutex> lk(mut_);
        queue_.push(std::move(value));
        cond_.notify_one();
    }

    // In-place construction
    template <class... Args>
    void emplace(Args &&... args)
    {
        std::lock_guard<std::mutex> lk(mut_);
        queue_.emplace(std::forward<Args>(args)...);
        cond_.notify_one();
    }

    // Try to pop immediately. Returns false if queue empty.
    bool try_pop(T &out)
    {
        std::lock_guard<std::mutex> lk(mut_);
        if (queue_.empty())
            return false;
        out = std::move(queue_.front());
        queue_.pop();
        return true;
    }

    // Wait until an element is available and pop it. Returns shared_ptr to value.
    std::shared_ptr<T> wait_and_pop()
    {
        std::unique_lock<std::mutex> lk(mut_);
        cond_.wait(lk, [this] { return !queue_.empty(); });
        std::shared_ptr<T> res(std::make_shared<T>(std::move(queue_.front())));
        queue_.pop();
        return res;
    }

    // Wait until an element is available and pop into out parameter.
    void wait_and_pop(T &out)
    {
        std::unique_lock<std::mutex> lk(mut_);
        cond_.wait(lk, [this] { return !queue_.empty(); });
        out = std::move(queue_.front());
        queue_.pop();
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lk(mut_);
        return queue_.empty();
    }

    std::size_t size() const
    {
        std::lock_guard<std::mutex> lk(mut_);
        return queue_.size();
    }

    // Notify one waiting thread (useful if you want to wake up for shutdown)
    void notify_one()
    {
        cond_.notify_one();
    }

private:
    mutable std::mutex mut_;
    std::queue<T> queue_;
    std::condition_variable cond_;
};

#endif // THREAD_SAFE_QUEUE_H
