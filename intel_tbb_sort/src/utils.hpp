#ifndef UTILS
#define UTILS

#include <boost/filesystem.hpp>
#include <boost/thread.hpp>
#include <utility>
#include <string>

using boost::filesystem::path;

typedef std::vector<std::string> vstr;
typedef std::vector<path> vpath;
typedef std::vector<int> vint;

namespace utils {

template <typename T>
void resize_and_clear(std::vector<T> & v, size_t size, T value)
{
    v.resize(size);
    std::fill(v.begin(), v.end(), value);
}

template <typename T>
void accumulate_sum(std::vector<T> & v, size_t end)
{
    typedef typename std::vector<T> vT;
    size_t accum = 0;
    for (typename vT::iterator it = v.begin(); it != v.begin() + end; ++it) {
        accum += *it;
        *it = accum;
    }
}

size_t get_optimal_thread_count();
void serialize(int number, std::fstream & output);
void serialize(vint const & vec, std::fstream & output);
void serialize(vpath const & vec, std::fstream & output);
int deserialize_number(std::fstream & input);
int deserialize_number(std::fstream & input, size_t offset);
std::string deserialize_string(std::fstream & input, size_t offset, size_t length);
bool pair_str_int_comparator(std::pair<std::string, size_t> const & first, std::pair<std::string, size_t> const & second);
bool starts_with(std::pair<std::string, size_t> const & main, std::string const & pattern);
}

#endif // UTILS
