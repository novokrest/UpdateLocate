#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <vector>
#include <set>
#include "file_manager.hpp"
#include "suffix_array_builder.hpp"
#include "pattern_finder.hpp"
#include <boost/archive/binary_oarchive.hpp>

using std::cout;
using std::cin;
using std::endl;
using boost::archive::binary_oarchive;
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

    suffix_array_builder sa_builder;
    file_manager file_manager(root_dir);
    file_manager.search(sa_builder);
    sa_builder.build_suffix_array();

    if (!file_manager.is_found()) {
        cout << "There are no files in specified folder!" << endl;
        return 0;
    }

    binary_oarchive oar(output);
    file_manager.serialize(oar);
    sa_builder.serialize(oar);

    return 0;
}


