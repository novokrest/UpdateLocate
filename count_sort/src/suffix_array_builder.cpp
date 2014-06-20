#include "suffix_array_builder.hpp"
#include "utils.hpp"
#include <algorithm>

typedef unsigned char uchar;

static const size_t ALPHABET_SIZE = 256;

suffix_array_builder::suffix_array_builder()
{}

suffix_array_builder::suffix_array_builder(std::string && text)
    : text_(text), sorted_indexs_(text_.size()), sorted_indexs_temp_(text_.size()), buckets_(text_.size()), symbols_count_(text_.size())
{}

void suffix_array_builder::set_text(std::string const & text)
{
    text_ = text;
}

void suffix_array_builder::build_suffix_array()
{
    sort_suffixes();
}

void suffix_array_builder::sort_suffixes()
{
    count_sort_init();

    thread_pool thread_pool(utils::get_optimal_thread_count());
    vpair borders(thread_pool.get_size());
    calculate_borders(borders);

    const size_t TEXT_SIZE = text_.size();
    for (size_t step = 1; step <= TEXT_SIZE; step <<= 1) {

        std::fill(symbols_count_.begin(), symbols_count_.end(), 0);
        run_multithread_calculate_frequencies(thread_pool, borders, step);

        for (int i = TEXT_SIZE - 1; i >= 0; --i) {
            sorted_indexs_[--symbols_count_[buckets_[sorted_indexs_temp_[i]]]] = sorted_indexs_temp_[i];
        }
        buckets_temp_[sorted_indexs_[0]] = 0;
        classes_ = 1;
        for (size_t i = 1; i < TEXT_SIZE; ++i) {
            size_t mid = (sorted_indexs_[i] + step) % TEXT_SIZE;
            size_t mid_prev = (sorted_indexs_[i - 1] + step) % TEXT_SIZE;
            if (buckets_[sorted_indexs_[i]] != buckets_[sorted_indexs_[i - 1]]
                    || buckets_[mid] != buckets_[mid_prev]) {
                ++classes_;
            }
            buckets_temp_[sorted_indexs_[i]] = classes_ - 1;
        }
        std::copy(buckets_temp_.begin(), buckets_temp_.end(), buckets_.begin());
    }
}

void suffix_array_builder::calculate_borders(vpair & borders)
{
    const size_t TEXT_SIZE = text_.size();
    size_t part = TEXT_SIZE / borders.size();
    size_t current_pos = 0;
    for (size_t i = 0; i < borders.size() - 1; ++i) {
        size_t start = current_pos;
        current_pos += part;
        size_t end = current_pos;
        borders[i] = std::pair<size_t, size_t>(start, end);
    }
    borders[borders.size() - 1] = std::pair<size_t, size_t>(current_pos, TEXT_SIZE);
}

void suffix_array_builder::count_sort_init()
{
    const size_t TEXT_SIZE = text_.size();
    utils::resize_and_clear(sorted_indexs_, TEXT_SIZE, 0);
    utils::resize_and_clear(sorted_indexs_temp_, TEXT_SIZE, 0);
    utils::resize_and_clear(buckets_, TEXT_SIZE, 0);
    utils::resize_and_clear(buckets_temp_, TEXT_SIZE, 0);
    utils::resize_and_clear(symbols_count_, std::max(TEXT_SIZE, ALPHABET_SIZE), 0);

    for (std::string::const_iterator symb_it = text_.begin(); symb_it != text_.end(); ++symb_it) {
        ++symbols_count_[(uchar)*symb_it];
    }

    utils::accumulate_sum(symbols_count_, ALPHABET_SIZE);
    for (size_t i = 0; i < sorted_indexs_.size(); ++i) {
        uchar current_symbol = text_[i];
        sorted_indexs_[--symbols_count_[current_symbol]] = i;
    }

    buckets_[sorted_indexs_[0]] = 0;
    classes_ = 1;
    for (size_t i = 1; i < buckets_.size(); ++i) {
        if (text_[sorted_indexs_[i]] != text_[sorted_indexs_[i - 1]]) {
            ++classes_;
        }
        buckets_[sorted_indexs_[i]] = classes_ - 1;
    }
}

void suffix_array_builder::run_multithread_calculate_frequencies(thread_pool & thread_pool, vpair const & borders, size_t step)
{
    future_queue futures;
    for (size_t i = 0; i < borders.size(); ++i) {
        futures.push(thread_pool.add_task(boost::bind(&suffix_array_builder::calculate_frequencies, this, borders[i].first, borders[i].second, step)));
    }

    while(!futures.empty()) {
        futures.front().wait();
        futures.pop();
    }
}

void suffix_array_builder::calculate_frequencies(size_t start, size_t end, size_t step)
{
    const size_t TEXT_SIZE = text_.size();
    vint sorted_indexs_local(TEXT_SIZE);
    vint symbols_count_local(TEXT_SIZE);

    for (size_t i = start; i != end; ++i) {
        sorted_indexs_local[i] = sorted_indexs_[i] - step;
        if (sorted_indexs_local[i] < 0) {
            sorted_indexs_local[i] += TEXT_SIZE;
        }
    }
    for (size_t i = start; i != end; ++i) {
        ++symbols_count_local[buckets_[sorted_indexs_local[i]]];
    }

    utils::accumulate_sum<int>(symbols_count_local, classes_);

    for (size_t i = start; i != end; ++i) {
        sorted_indexs_temp_[i] = sorted_indexs_local[i];
    }

    write_symbols_count(symbols_count_local);
}

void suffix_array_builder::write_symbols_count(vint const & results)
{
    std::lock_guard<std::mutex> locker(lock_);
    for (size_t i = 0; i < classes_; ++i) {
        symbols_count_[i] += results[i];
    }
}

vint suffix_array_builder::get_suffix_array()
{
    return std::move(sorted_indexs_);
}

void suffix_array_builder::create_expensive_suffix_array()
{
    for (vint::const_iterator it = sorted_indexs_.begin(); it != sorted_indexs_.end(); ++it) {
        std::string cyclic_suffix = text_.substr(*it) + text_.substr(0, *it);
        suffix_array_.push_back(cyclic_suffix);
    }
}
