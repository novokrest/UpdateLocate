#ifndef FILE_MANAGER
#define FILE_MANAGER

#include "thread_pool.hpp"
#include "suffix_array_builder.hpp"
#include <boost/filesystem.hpp>
#include <boost/thread.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <mutex>
#include <vector>
#include <string>
#include <queue>
#include <utility>

using namespace boost::filesystem;

using boost::archive::binary_oarchive;
using boost::archive::binary_iarchive;

typedef std::queue<boost::shared_future<void>> future_queue;
typedef std::vector<int> vint;
typedef std::vector<path> vpath;

class file_manager
{
    boost::mutex lock_;

    path root_dir_;
    vstr files_;

    vpath visited_dirs_;
    vint filenames_map_;
    future_queue remaining_dirs_;

    void get_files_from_dir(path const & dir, thread_pool & thread_pool, suffix_array_builder & sa_builder);
    void safe_add_file(path & file, suffix_array_builder & sa_builder);
    void safe_add_visited_dir(path const & dir, thread_pool & thread_pool, suffix_array_builder & sa_builder);

public:
    file_manager();
    file_manager(path const & root_dir);

    void set_path(path const & root_dir);
    void search(suffix_array_builder & sa_builder);
    bool is_found() const;
    vstr get_files();
    void serialize(binary_oarchive & oar);
    void deserialize(binary_iarchive & iar);
};

#endif // FILE_MANAGER
