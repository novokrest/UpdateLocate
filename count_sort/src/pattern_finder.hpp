#ifndef PATTERN_FINDER
#define PATTERN_FINDER

#include "bwt.hpp"
#include "deserializer.hpp"
#include <vector>
#include <string>
#include <set>

typedef unsigned char uchar;
typedef std::vector<int> vint;
typedef std::vector<vint> vvint;
typedef std::vector<std::string> vstr;

class pattern_finder
{
    deserializer* data_;

    int lower_bound_;
    int upper_bound_;

    void init_bounds(uchar symbol);
    void refresh_bounds(uchar symbol);

public:
    pattern_finder();
    pattern_finder(char* index_file_name);
    void find(std::string const & pattern);
    vstr get_results();
    void print_results();
    int get_lower_bound() const;
    int get_upper_bound() const;
};

#endif // PATTERN_FINDER
