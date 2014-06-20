#ifndef SUFFIX_ARRAY_BUILDER
#define SUFFIX_ARRAY_BUILDER

#include <mutex>
#include <boost/thread.hpp>
#include <vector>
#include <string>
#include <functional>
#include <utility>
#include <queue>
#include "thread_pool.hpp"

typedef std::vector<int> vint;
typedef std::vector<std::pair<size_t, size_t>> vpair;
typedef std::vector<std::string> vstr;
typedef std::vector<boost::thread> vthread;
typedef std::queue<boost::shared_future<void>> future_queue;

class suffix_array_builder
{
    std::mutex lock_;
    vthread threads_;

    std::string text_;
    vint sorted_indexs_;
    vint sorted_indexs_temp_;
    vint buckets_;
    vint buckets_temp_;
    vint symbols_count_;
    size_t classes_;
    size_t step_;

    void create_tasks();

    void sort_suffixes();
    void count_sort_init();
    void calculate_borders(vpair & borders);
    void write_symbols_count(vint const & results);
    void run_multithread_calculate_frequencies(thread_pool & thread_pool, vpair const & borders, size_t step);
    void calculate_frequencies(size_t start, size_t end, size_t step);

public:
    suffix_array_builder();
    suffix_array_builder(std::string && text);
    void set_text(std::string const & text);
    void build_suffix_array();
    vint get_suffix_array();

    vstr suffix_array_;
    void create_expensive_suffix_array();
};

#endif // SUFFIX_ARRAY_BUILDER
