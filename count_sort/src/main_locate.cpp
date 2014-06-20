#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <vector>
#include <set>
#include "file_manager.hpp"
#include "file_mapper.hpp"
#include "suffix_array_builder.hpp"
#include "bwt.hpp"
#include "deserializer.hpp"
#include "pattern_finder.hpp"

using std::cout;
using std::cin;
using std::endl;
using namespace boost::filesystem;

typedef std::vector<int> vint;
typedef std::vector<path> vpath;

static size_t const ERROR_ = 1;


int main(int args, char** argv)
{
    if (args < 4
            || strcmp(argv[1], "--database")) {
        cout << "Incorrect arguments!\nTry: locate --database FILE PATTERN" << endl;
        return ERROR_;
    }

    path index_file(argv[2]);
    if (!exists(index_file)) {
        cout << "Specified file doesn't exist! Try again..." << endl;
        return ERROR_;
    }

    pattern_finder pattern_finder(argv[2]);
    pattern_finder.find(argv[3]);
    pattern_finder.print_results();

    return 0;
}

