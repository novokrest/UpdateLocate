#include "file_mapper.hpp"
#include "utils.hpp"

static char const SENTINEL = '\1';

file_mapper::file_mapper(std::string const & filenames, vint const & suffix_array)
    : files_map_(filenames.size(), -1), sorted_suffix_to_file_map_(suffix_array.size())
{
    size_t file_number = 0;
    for (size_t i = 0; i < filenames.size(); ++i) {
        if (filenames[i] == SENTINEL) {
            ++file_number;
        }
        else {
            files_map_[i] = file_number;
        }
    }

    for (size_t i = 0; i < suffix_array.size(); ++i) {
        sorted_suffix_to_file_map_[i] = files_map_[suffix_array[i]];
    }
}

int file_mapper::get_file_number(size_t index) const
{
    return files_map_[index];
}

void file_mapper::serialize(std::fstream & output) const
{
    utils::serialize(sorted_suffix_to_file_map_, output);
}
