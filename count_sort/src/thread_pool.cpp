#include "thread_pool.hpp"


thread_pool::thread_pool(size_t pool_size)
    : pool_size_(pool_size), available_(pool_size), is_running_(true)
{
    for (size_t i = 0; i < pool_size_; ++i) {
        threads_.create_thread(boost::bind(&thread_pool::main_loop, this));
    }
}

void thread_pool::main_loop()
{
    while (is_running_)
    {
        boost::unique_lock<boost::mutex> locker(lock_);
        while (tasks_.empty() && is_running_) {
            queue_check_.wait(locker);
        }

        if (stop_condition()) {
            queue_check_.notify_all();
            break;
        }

        {
            boost::function<void()> task = tasks_.front();
            tasks_.pop();
            --available_;
            locker.unlock();
            task();
        }

        locker.lock();
        ++available_;
    }
}

bool thread_pool::stop_condition()
{
    return get_available() == get_size() && is_empty() && !is_running_;
}

void thread_pool::push_task(boost::function<void()> task)
{
    boost::lock_guard<boost::mutex> locker(lock_);
    tasks_.push(task);
    queue_check_.notify_all();
}

size_t thread_pool::get_size() const
{
    return pool_size_;
}

size_t thread_pool::get_available() const
{
    return available_;
}

bool thread_pool::is_empty() const
{
    return tasks_.empty();
}

void thread_pool::kill() 
{
    is_running_ = false;
    queue_check_.notify_all();
}

thread_pool::~thread_pool()
{
    {
        boost::unique_lock<boost::mutex> locker(lock_);
        is_running_ = false;
    }
    queue_check_.notify_all();
    threads_.join_all();
}
