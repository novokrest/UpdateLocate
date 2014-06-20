#ifndef FILE_MANAGER
#define FILE_MANAGER

#include "thread_pool.hpp"
#include <boost/filesystem.hpp>
#include <boost/thread.hpp>
#include <mutex>
#include <vector>
#include <string>
#include <queue>

using namespace boost::filesystem;

typedef std::queue<boost::shared_future<void>> future_queue;
typedef std::vector<int> vint;
typedef std::vector<path> vpath;

class file_manager
{
    boost::mutex lock_;

    path root_dir_;
    vpath files_;
    vpath visited_dirs_;
    vint filenames_map_;
    future_queue remaining_dirs_;

    void get_files_from_dir(path const & dir, thread_pool & thread_pool);
    void safe_add_file(path const & file);
    void safe_add_visited_dir(path const & dir, thread_pool & thread_pool);

public:
    file_manager(path const & root_dir);
    void search();
    bool is_found() const;
    size_t files_count() const;
    std::string get_filename(size_t index) const;
    std::string get_full_filename(size_t index) const;
    std::string get_filenames_concatenation() const;
    void serialize(std::fstream & output) const;
};

#endif // FILE_MANAGER
