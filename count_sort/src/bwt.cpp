#include "bwt.hpp"
#include "utils.hpp"

typedef unsigned char uchar;

static size_t const ALPHABET_SIZE = 256;

burrows_wheeler_transform::burrows_wheeler_transform(std::string const & text, vint const & sorted_indexs)
    : text_size_(text.size()), start_pos_(ALPHABET_SIZE, -1), occurrences_(ALPHABET_SIZE, vint(text.size(), 0))
{
    construct_symbols_start_pos(text, sorted_indexs);
    construct_symbols_occurrences(text, sorted_indexs);
}

void burrows_wheeler_transform::construct_symbols_start_pos(std::string const & text, vint const & sorted_indexs)
{
    uchar last_symbol = text[sorted_indexs[0]];
    start_pos_[last_symbol] = 0;
    for (size_t i = 0; i < sorted_indexs.size(); ++i) {
        uchar current_symbol = text[sorted_indexs[i]];
        if (current_symbol != last_symbol) {
            start_pos_[current_symbol] = i;
            last_symbol = current_symbol;
        }
    }
}

void burrows_wheeler_transform::construct_symbols_occurrences(std::string const & text, vint const & sorted_indexs)
{
    vint last_occurrences_indexs(ALPHABET_SIZE, -1);
    vint occurrences_current_(ALPHABET_SIZE, 0);
    for (size_t i = 0; i < sorted_indexs.size(); ++i) {
        int prev_symbol_index = (sorted_indexs[i] - 1) < 0 ? sorted_indexs[i] - 1 + text.size() : sorted_indexs[i] - 1;
        uchar current_symbol = text[prev_symbol_index];
        std::fill(occurrences_[current_symbol].begin() + (last_occurrences_indexs[current_symbol] + 1), occurrences_[current_symbol].begin() + i, occurrences_current_[current_symbol]);
        ++occurrences_current_[current_symbol];
        occurrences_[current_symbol][i] = occurrences_current_[current_symbol];
        last_occurrences_indexs[current_symbol] = i;
    }

    for (size_t i = 0; i < ALPHABET_SIZE; ++i) {
        std::fill(occurrences_[i].begin() + (last_occurrences_indexs[i] + 1), occurrences_[i].end(), occurrences_current_[i]);
    }
}

size_t burrows_wheeler_transform::get_text_size() const
{
    return text_size_;
}

vint const & burrows_wheeler_transform::get_symbols_start_pos() const
{
    return start_pos_;
}

vvint const & burrows_wheeler_transform::get_symbols_occurrences() const
{
    return occurrences_;
}

void burrows_wheeler_transform::serialize(std::fstream & file) const
{
    utils::serialize(ALPHABET_SIZE, file);
    utils::serialize(text_size_, file);

    utils::serialize(start_pos_, file);
    for (vvint::const_iterator it = occurrences_.begin(); it != occurrences_.end(); ++it) {
        utils::serialize(*it, file);
    }
}
