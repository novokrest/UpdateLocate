#include "converter.hpp"

static size_t const BITS_IN_BYTE = 8;

void number_to_bytes(int number, char* bytes, size_t bytes_count)
{
    for (size_t i = 0; i < bytes_count; ++i) {
        bytes[i] = number >> BITS_IN_BYTE * i;
    }
}

int number_from_bytes(char* bytes, size_t bytes_count)
{
    int number = 0;
    for (size_t i = 0; i < bytes_count; ++i) {
        unsigned char byte = bytes[i];
        number += (byte << BITS_IN_BYTE * i);
    }

    return number;
}
