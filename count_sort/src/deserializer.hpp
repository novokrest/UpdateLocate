#ifndef DESERIALIZER
#define DESERIALIZER

#include <fstream>

class deserializer
{
    std::fstream index_file_;

    size_t alphabet_size_;
    size_t text_size_;
    size_t files_count_;

    size_t start_pos_offset_;
    size_t occurrences_offset_;
    size_t suffix_to_file_map_offset_;
    size_t files_information_offset_;
    size_t files_names_offset_;

    void init();

public:
    deserializer(const char* index_file_name);

    size_t text_size() const;
    size_t alphabet_size() const;
    int start_pos(size_t symbol);
    int occurrences_count(size_t symbol, size_t index);
    int get_file_number(size_t suffix_index);
    std::string get_file_name(size_t file_number);
};

#endif // DESERIALIZER
