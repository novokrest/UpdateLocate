#include "deserializer.hpp"
#include "utils.hpp"

static const size_t BYTES_FOR_NUMBER = 4;
static const size_t BYTES_FOR_SYMBOL = 1;

deserializer::deserializer(const char* index_file_name)
{
    index_file_.open(index_file_name, std::fstream::in | std::fstream::binary);
    if (!index_file_.fail()) {
        init();
    }
}

void deserializer::init() {
    alphabet_size_ = utils::deserialize_number(index_file_);
    text_size_ = utils::deserialize_number(index_file_);
    start_pos_offset_ = 2 * BYTES_FOR_NUMBER;
    occurrences_offset_ = start_pos_offset_ + alphabet_size_ * BYTES_FOR_NUMBER;

    suffix_to_file_map_offset_ = occurrences_offset_ + alphabet_size_ * text_size_ * BYTES_FOR_NUMBER;

    files_count_ = utils::deserialize_number(index_file_, suffix_to_file_map_offset_ + text_size_ * BYTES_FOR_NUMBER);
    files_information_offset_ = suffix_to_file_map_offset_ + text_size_ * BYTES_FOR_NUMBER + BYTES_FOR_NUMBER;
    files_names_offset_ = files_information_offset_ + (files_count_ + 1) * BYTES_FOR_NUMBER;
}

size_t deserializer::text_size() const
{
    return text_size_;
}

size_t deserializer::alphabet_size() const
{
    return alphabet_size_;
}

int deserializer::start_pos(size_t symbol)
{
    int symbol_start_pos = utils::deserialize_number(index_file_, start_pos_offset_ + symbol * BYTES_FOR_NUMBER);
    return symbol_start_pos;
}

int deserializer::occurrences_count(size_t symbol, size_t index)
{
    int symbol_occurrences_count = utils::deserialize_number(index_file_,
                                                             occurrences_offset_ + (symbol * text_size_ + index) * BYTES_FOR_NUMBER);
    return symbol_occurrences_count;
}

int deserializer::get_file_number(size_t suffix_index)
{
    int file_number = utils::deserialize_number(index_file_, suffix_to_file_map_offset_ + suffix_index * BYTES_FOR_NUMBER);
    return file_number;
}

std::string deserializer::get_file_name(size_t file_number)
{
    int file_name_offset = utils::deserialize_number(index_file_, files_information_offset_ + file_number * BYTES_FOR_NUMBER);
    int next_file_name_offset = utils::deserialize_number(index_file_, files_information_offset_ + (file_number + 1) * BYTES_FOR_NUMBER);
    int file_name_length = next_file_name_offset - file_name_offset;

    std::string file_name = utils::deserialize_string(index_file_, files_names_offset_ + file_name_offset, file_name_length);
    return file_name;
}
