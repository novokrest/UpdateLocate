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
    if ((args < 5)
            || strcmp(argv[1], "--database-root")
            || strcmp(argv[3], "--output")) {
        cout << "Incorrect arguments!\nTry: updatedb --database-root PATH --output FILE" << endl;
        return ERROR_;
    }

    path root_dir(argv[2]);
    if (!exists(root_dir) || !is_directory(root_dir)) {
        cout << "Dir doesn't exist! Try again..." << endl;
        return ERROR_;
    }

    std::fstream output(argv[4], std::fstream::out | std::fstream::trunc | std::fstream::binary);
    if (output.fail()) {
        cout << "Couldn't open specified file! Try again..." << endl;
        return ERROR_;
    }

    file_manager file_manager(root_dir);
    file_manager.search();
    if (!file_manager.is_found()) {
        cout << "There are no files in specified folder!" << endl;
        return 0;
    }
    std::string filenames = file_manager.get_filenames_concatenation();

    suffix_array_builder sa_builder;
    sa_builder.set_text(filenames);
    sa_builder.build_suffix_array();

    vint suffix_array = sa_builder.get_suffix_array();
    file_mapper file_mapper(filenames, suffix_array);
    burrows_wheeler_transform bwt(filenames, suffix_array);

    bwt.serialize(output);
    file_mapper.serialize(output);
    file_manager.serialize(output);

    return 0;
}


