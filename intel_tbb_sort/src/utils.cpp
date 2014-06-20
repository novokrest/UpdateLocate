#include "utils.hpp"
#include "converter.hpp"
#include <boost/thread.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <thread>
#include <fstream>

using std::cout;
using std::cin;
using std::endl;

static size_t const BYTES_FOR_NUMBER = 4;

size_t utils::get_optimal_thread_count()
{
    return boost::thread::hardware_concurrency();
}

void utils::serialize(int number, std::fstream & output)
{
    char bytes[BYTES_FOR_NUMBER];
    number_to_bytes(number, bytes, BYTES_FOR_NUMBER);
    output.write(bytes, BYTES_FOR_NUMBER);
}

void utils::serialize(vint const & vec, std::fstream & output)
{
    for (vint::const_iterator it = vec.begin(); it != vec.end(); ++it) {
        utils::serialize(*it, output);
    }
}

void utils::serialize(vpath const & vec, std::fstream & output)
{
    for (vpath::const_iterator it = vec.begin(); it != vec.end(); ++it) {
        output.write(it->generic_string().c_str(), it->generic_string().length());
    }
}

int utils::deserialize_number(std::fstream & input)
{
    char bytes[BYTES_FOR_NUMBER];
    input.read(bytes, BYTES_FOR_NUMBER);
    return number_from_bytes(bytes, BYTES_FOR_NUMBER);
}

int utils::deserialize_number(std::fstream & input, size_t offset)
{
    input.seekg(offset, std::ios::beg);
    return utils::deserialize_number(input);
}

std::string utils::deserialize_string(std::fstream & input, size_t offset, size_t length)
{
    char* cstr = new char[length];
    input.seekg(offset, std::fstream::beg);
    input.read(cstr, length);

    std::string str(cstr, length);
    return str;
}

bool utils::pair_str_int_comparator(std::pair<std::string, size_t> const & pair1, std::pair<std::string, size_t> const & pair2)
{
    return pair1.first < pair2.first;
}

bool utils::starts_with(std::pair<std::string, size_t> const & main, std::string const & pattern)
{
    return main.first.compare(pattern) < 0;
}
