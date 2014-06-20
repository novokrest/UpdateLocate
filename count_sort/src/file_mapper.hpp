#ifndef FILE_MAPPER
#define FILE_MAPPER

#include "file_manager.hpp"
#include <vector>
#include <string>

typedef std::vector<int> vint;

class file_mapper
{
    vint files_map_;
    vint sorted_suffix_to_file_map_;

public:
    file_mapper(std::string const & filenames, vint const & suffix_array);
    int get_file_number(size_t suffix_index) const;
    void serialize(std::fstream & output) const;
};

#endif // FILE_MAPPER
