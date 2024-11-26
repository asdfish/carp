#ifndef CARP_H
#define CARP_H

#include <stddef.h>

struct CarpOption {
  const char* flag_long;
  char flag_short;
};

enum CarpArgumentType {
  FLAG,
  KEY,
};

typedef int (*carp_callback) (char flag_short, enum CarpArgumentType argument_type, const char* argument, void* user_data);

extern int carp_parse(int argc, const char** restrict argv, const struct CarpOption* restrict options, size_t options_length, carp_callback callback, void* user_data);

#endif
