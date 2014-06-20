#ifndef SUFFIX_ARRAY_BUILDER
#define SUFFIX_ARRAY_BUILDER

#include <mutex>
#include <boost/thread.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <vector>
#include <string>
#include <functional>
#include <utility>
#include <queue>
#include "thread_pool.hpp"

using std::string;
using boost::archive::binary_oarchive;
using boost::archive::binary_iarchive;

typedef std::vector<int> vint;
typedef std::vector<std::pair<size_t, size_t>> vpair;
typedef std::vector<std::string> vstr;
typedef std::vector<boost::thread> vthread;
typedef std::queue<boost::shared_future<void>> future_queue;
typedef std::vector<std::pair<std::string, size_t>> vpair_str_int;

class suffix_array_builder
{
    vpair_str_int suffixes_;

public:
    void add_suffixes(std::string const & text, size_t number);
    void build_suffix_array();
    vpair_str_int get_suffix_array();
    void serialize(binary_oarchive & oar);
    void deserialize(binary_iarchive & iar);
};

#endif // SUFFIX_ARRAY_BUILDER
