#include "file_manager.hpp"
#include "thread_pool.hpp"
#include "utils.hpp"

static char const SENTINEL = '\1';

file_manager::file_manager(path const & root_dir)
    : root_dir_(root_dir)
{}

void file_manager::search()
{
    thread_pool thread_pool(boost::thread::hardware_concurrency());
    boost::shared_future<void> root_dir_future = thread_pool.add_task(boost::bind(&file_manager::get_files_from_dir, this, root_dir_, boost::ref(thread_pool)));
    remaining_dirs_.push(root_dir_future);
    while(!remaining_dirs_.empty())
    {
        boost::shared_future<void> dir_future = remaining_dirs_.front();
        dir_future.wait();
        remaining_dirs_.pop();
    }
}

void file_manager::get_files_from_dir(path const & dir, thread_pool & thread_pool)
{
    vpath dir_entries;
    copy(directory_iterator(dir), directory_iterator(), std::back_inserter(dir_entries));
    
    for (vpath::const_iterator it = dir_entries.begin(); it != dir_entries.end(); ++it) {
        if (is_symlink(*it)) {
            continue;
        }

        if (is_regular_file(*it)) {
            safe_add_file(*it);
            continue;
        }

        if (is_directory(*it)) {
            safe_add_visited_dir(*it, thread_pool);
        }
    }
}

void file_manager::safe_add_file(path const & file)
{
    boost::lock_guard<boost::mutex> locker(lock_);
    files_.push_back(file);
}

void file_manager::safe_add_visited_dir(path const & dir, thread_pool & thread_pool)
{
    boost::lock_guard<boost::mutex> locker(lock_);
    boost::shared_future<void> dir_future = thread_pool.add_task(boost::bind(&file_manager::get_files_from_dir, this, dir, boost::ref(thread_pool)));
    remaining_dirs_.push(dir_future);
}

bool file_manager::is_found() const
{
    return !files_.empty();
}

size_t file_manager::files_count() const
{
    return files_.size();
}

std::string file_manager::get_filename(size_t index) const
{
    return files_[index].filename().string();
}

std::string file_manager::get_full_filename(size_t index) const
{
    return files_[index].generic_string();
}

std::string file_manager::get_filenames_concatenation() const
{
    std::string filenames_concatenation;
    for (vpath::const_iterator it = files_.begin(); it != files_.end(); ++it) {
        filenames_concatenation += it->filename().string() + SENTINEL;
    }

    return std::move(filenames_concatenation);
}

void file_manager::serialize(std::fstream & output) const 
{
    vint offsets(files_.size() + 1);
    int accum_offset = 0;
    for (size_t i = 0; i < files_.size(); ++i) {
        offsets[i] = accum_offset;
        accum_offset += files_[i].generic_string().length();
    }
    offsets[offsets.size() - 1] = accum_offset;

    utils::serialize(files_.size(), output);
    utils::serialize(offsets, output);
    utils::serialize(files_, output);
}


