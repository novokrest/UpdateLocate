#include "pattern_finder.hpp"
#include <boost/filesystem.hpp>
#include <iostream>
#include <iomanip>

using namespace boost::filesystem;
using std::cout;
using std::endl;
using std::setw;

typedef std::set<int> sint;

pattern_finder::pattern_finder()
    : lower_bound_(-1), upper_bound_(-1)
{}

pattern_finder::pattern_finder(char* index_file_name)
    : data_(new deserializer(index_file_name)),
      lower_bound_(-1), upper_bound_(-1)
{}

void pattern_finder::find(std::string const & pattern)
{
    init_bounds(*(pattern.end() - 1));
    for (std::string::const_reverse_iterator symb_it = pattern.rbegin() + 1; symb_it != pattern.rend(); ++symb_it) {
        if (lower_bound_ < 0 || upper_bound_ < 0 || lower_bound_ >= upper_bound_) {
            return;
        }
        refresh_bounds(*symb_it);
    }
}

void pattern_finder::init_bounds(uchar symbol)
{
    lower_bound_ = data_->start_pos(symbol);/* symbols_start_pos_[symbol];*/
    if (lower_bound_ < 0) {
        return;
    }
    upper_bound_ = data_->text_size();

    for (size_t s = symbol + 1; s < data_->alphabet_size(); ++s) {
        if (data_->start_pos(s) > -1) {
            upper_bound_ = data_->start_pos(s);
            break;
        }
    }
}

void pattern_finder::refresh_bounds(uchar symbol)
{
    int start_pos = data_->start_pos(symbol);
    int lower_offset = lower_bound_ > 0 ? data_->occurrences_count(symbol, lower_bound_ - 1) : 0;
    int upper_offset = upper_bound_ > 0 ? data_->occurrences_count(symbol, upper_bound_ - 1) : 0;
    lower_bound_ = start_pos + lower_offset;
    upper_bound_ = start_pos + upper_offset;
    if (start_pos < 0 || lower_bound_ >= upper_bound_) {
        lower_bound_ = -1;
        upper_bound_ = -1;
    }
}

vstr pattern_finder::get_results()
{
    sint file_numbers;
    for (int i = lower_bound_; i < upper_bound_; ++i) {
        int file_number = data_->get_file_number(i);
        file_numbers.insert(file_number);
    }

    vstr file_names;
    for (sint::const_iterator it = file_numbers.begin(); it != file_numbers.end(); ++it) {
        file_names.push_back(data_->get_file_name(*it));
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

int pattern_finder::get_lower_bound() const
{
    return lower_bound_;
}

int pattern_finder::get_upper_bound() const
{
    return upper_bound_;
}

