#ifndef PATTERN_FINDER
#define PATTERN_FINDER

#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <utility>

typedef unsigned char uchar;
typedef std::vector<size_t> vsize_t;
typedef std::vector<std::string> vstr;
typedef std::vector<std::pair<std::string, size_t>> vpair_str_int;

class pattern_finder
{
    std::string last_pattern_;
    vstr files_;
    vpair_str_int sorted_suffixes_;

    vpair_str_int::iterator lower_bound_;
    vpair_str_int::iterator upper_bound_;

public:
    pattern_finder(char* index_file_name);
    void find(std::string const & pattern);
    vstr get_results();
    void print_results();
};

#endif // PATTERN_FINDER
