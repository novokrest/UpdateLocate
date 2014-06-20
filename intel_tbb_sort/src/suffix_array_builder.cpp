#include "suffix_array_builder.hpp"
#include "utils.hpp"
#include <boost/serialization/vector.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/string.hpp>
#include <tbb/parallel_sort.h>
#include <algorithm>


typedef unsigned char uchar;

static const size_t ALPHABET_SIZE = 256;

void suffix_array_builder::add_suffixes(string const & text, size_t number)
{
    for (size_t i = 0; i < text.length(); ++i) {
        suffixes_.push_back(std::pair<std::string, size_t>(text.substr(i), number));
    }
}

void suffix_array_builder::build_suffix_array()
{
    tbb::parallel_sort (suffixes_.begin(), suffixes_.end(), utils::pair_str_int_comparator);
}

vpair_str_int suffix_array_builder::get_suffix_array()
{
    return std::move(suffixes_);
}

void suffix_array_builder::serialize(binary_oarchive & oar)
{
    oar << suffixes_;
}

void suffix_array_builder::deserialize(binary_iarchive & iar)
{
    iar >> suffixes_;
}
