#ifndef BWT
#define BWT

#include <vector>
#include <string>
#include <fstream>

typedef std::vector<int> vint;
typedef std::vector<vint> vvint;

class burrows_wheeler_transform
{
    size_t const text_size_;
    vint start_pos_;
    vvint occurrences_;

    void construct_symbols_start_pos(std::string const & text, vint const & sorted_indexs);
    void construct_symbols_occurrences(std::string const & text, vint const & sorted_indexs);

public:
    burrows_wheeler_transform();
    burrows_wheeler_transform(std::string const & text, vint const & sorted_indexs);
    void serialize(std::fstream & file) const;
    size_t get_text_size() const;
    vint const & get_symbols_start_pos() const;
    vvint const & get_symbols_occurrences() const;
};

#endif // BWT
