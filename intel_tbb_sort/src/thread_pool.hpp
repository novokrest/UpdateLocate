#ifndef THREAD_POOL
#define THREAD_POOL

#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <queue>

typedef std::queue<boost::function<void()>> fqueue;

class thread_pool
{
public:
    boost::mutex lock_;
    boost::condition_variable queue_check_;
    boost::thread_group threads_;
    fqueue tasks_;

    size_t pool_size_;
    size_t available_;
    bool is_running_;

    void main_loop();

    thread_pool(thread_pool const & tpn);
    bool stop_condition();
    void push_task(boost::function<void()> task);

public:
    thread_pool(size_t pool_size);
    virtual ~thread_pool();

    template <typename Task>
    boost::shared_future<void> add_task(Task task);

    size_t get_size() const;
    size_t get_available() const;
    bool is_empty() const;
    void kill();
};

template <typename Task>
boost::shared_future<void> thread_pool::add_task(Task task)
{
    boost::shared_ptr<boost::packaged_task<void>> packaged_task_ptr (new boost::packaged_task<void>([=](){ task(); }));
    boost::function<void()> sync_task = [=](){ (*packaged_task_ptr)(); };
    push_task(sync_task);

    return packaged_task_ptr->get_future();
}

#endif // THREAD_POOL
