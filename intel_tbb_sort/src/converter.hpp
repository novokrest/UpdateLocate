#ifndef CONVERTER
#define CONVERTER

#include <cstddef>

void number_to_bytes(int number, char* bytes, size_t bytes_count);
int number_from_bytes(char* bytes, size_t bytes_count);

#endif // CONVERTER
