#ifndef CARP_H
#define CARP_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdbool.h>
#include <stddef.h>

struct CarpOption {
  const char* flag_long;
  char flag_short;
  bool require_argument;
};

enum CarpArgumentType {
  FLAG,
  KEY,
};

typedef int (*carp_callback) (char flag_short, enum CarpArgumentType argument_type, const char* restrict argument, void* restrict user_data);
extern int carp_parse(int argc, const char** restrict argv, const struct CarpOption* restrict options, size_t options_length, carp_callback callback, void* user_data);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // CARP_H
