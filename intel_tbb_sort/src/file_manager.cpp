#include "file_manager.hpp"
#include "thread_pool.hpp"
#include "utils.hpp"
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>

static char const SENTINEL = '\1';

file_manager::file_manager()
{}

file_manager::file_manager(path const & root_dir)
    : root_dir_(root_dir)
{}

void file_manager::set_path(path const & root_dir)
{
    root_dir_ = root_dir;
}

void file_manager::search(suffix_array_builder & sa_builder)
{
    thread_pool thread_pool(boost::thread::hardware_concurrency());
    boost::shared_future<void> root_dir_future = thread_pool.add_task(boost::bind(&file_manager::get_files_from_dir, this, root_dir_, boost::ref(thread_pool), boost::ref(sa_builder)));
    remaining_dirs_.push(root_dir_future);
    while(!remaining_dirs_.empty())
    {
        boost::shared_future<void> dir_future = remaining_dirs_.front();
        dir_future.wait();
        remaining_dirs_.pop();
    }
}

void file_manager::get_files_from_dir(path const & dir, thread_pool & thread_pool, suffix_array_builder & sa_builder)
{
    vpath dir_entries;
    copy(directory_iterator(dir), directory_iterator(), std::back_inserter(dir_entries));
    
    for (vpath::iterator it = dir_entries.begin(); it != dir_entries.end(); ++it) {
        if (is_symlink(*it)) {
            continue;
        }

        if (is_regular_file(*it)) {
            safe_add_file(*it, sa_builder);
            continue;
        }

        if (is_directory(*it)) {
            safe_add_visited_dir(*it, thread_pool, sa_builder);
        }
    }
}

void file_manager::safe_add_file(path & file, suffix_array_builder & sa_builder)
{
    boost::lock_guard<boost::mutex> locker(lock_);
    size_t file_number = files_.size();
    std::string file_name = file.filename().string();
    files_.push_back(file.string());
    sa_builder.add_suffixes(file_name, file_number);
}

void file_manager::safe_add_visited_dir(path const & dir, thread_pool & thread_pool,  suffix_array_builder & sa_builder)
{
    boost::lock_guard<boost::mutex> locker(lock_);
    if (std::find(visited_dirs_.begin(), visited_dirs_.end(), dir) == visited_dirs_.end()) {
        visited_dirs_.push_back(dir);
        boost::shared_future<void> dir_future = thread_pool.add_task(boost::bind(&file_manager::get_files_from_dir, this, dir, boost::ref(thread_pool), boost::ref(sa_builder)));
        remaining_dirs_.push(dir_future);
    }
}

bool file_manager::is_found() const
{
    return !files_.empty();
}

vstr file_manager::get_files()
{
    return std::move(files_);
}

void file_manager::serialize(binary_oarchive & oar)
{
    oar << files_;
}

void file_manager::deserialize(binary_iarchive & iar)
{
    iar >> files_;
}
