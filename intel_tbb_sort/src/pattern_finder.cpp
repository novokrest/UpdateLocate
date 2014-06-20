#include "pattern_finder.hpp"
#include "utils.hpp"
#include <boost/filesystem.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <algorithm>
#include <iostream>
#include <iomanip>

using std::cout;
using std::endl;
using std::setw;
using boost::archive::binary_iarchive;
using namespace boost::filesystem;

typedef std::set<int> sint;


pattern_finder::pattern_finder(char* index_file_name)
{
    std::ifstream index_file(index_file_name);
    binary_iarchive iar(index_file);
    vpair_str_int pairs;
    iar >> files_;
    iar >> sorted_suffixes_;
}

void pattern_finder::find(std::string const & pattern)
{
    lower_bound_ = std::lower_bound(sorted_suffixes_.begin(), sorted_suffixes_.end(), pattern,
                                    [](std::pair<std::string, size_t> const & suff, std::string const & pattern) { return suff.first.compare(pattern) < 0; });
    upper_bound_ = std::upper_bound(sorted_suffixes_.begin(), sorted_suffixes_.end(), pattern,
                                    [](std::string const & pattern, std::pair<std::string, size_t> const & suff) { return suff.first.compare(pattern) > 0; });
    
    last_pattern_ = pattern;
}

vstr pattern_finder::get_results()
{
    sint file_numbers;
    for (vpair_str_int::iterator it = lower_bound_; it < sorted_suffixes_.end() && boost::starts_with(it->first, last_pattern_); ++it) {
        size_t file_number = it->second;
        file_numbers.insert(file_number);
    }

    vstr file_names;
    for (sint::const_iterator it = file_numbers.begin(); it != file_numbers.end(); ++it) {
        file_names.push_back(files_[*it]);
    }

    return std::move(file_names);
}

void pattern_finder::print_results()
{
    bool no_files = true;
    vstr file_names = get_results();

    cout << "Search results:" << endl;
    for (vstr::const_iterator it = file_names.begin(); it != file_names.end(); ++it) {
        path file(*it);
        if (exists(file)) {
            cout << setw(3) << file << endl;
            no_files = false;
        }
    }

    if (no_files) {
        cout << "No appropriate files..." << endl;
    }
}
