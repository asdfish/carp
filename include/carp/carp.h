#ifndef CARP_H
#define CARP_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdbool.h>
#include <stddef.h>

typedef struct {
  const char* flag_long;
  char flag_short;
  bool require_argument;
} carp_option_t;

typedef enum {
  CARP_ARGUMENT_TYPE_FLAG,
  CARP_ARGUMENT_TYPE_KEY,
} carp_argument_t;

#ifdef __cplusplus
typedef int (*carp_callback) (char flag_short, carp_argument_t argument_type, const char* argument, void* user_data);
extern int carp_parse(int argc, const char** argv, const carp_option_t* options, size_t options_length, carp_callback callback, void* user_data);
#else
typedef int (*carp_callback) (char flag_short, carp_argument_t argument_type, const char* restrict argument, void* restrict user_data);
extern int carp_parse(int argc, const char** restrict argv, const carp_option_t* restrict options, size_t options_length, carp_callback callback, void* restrict user_data);
#endif

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // CARP_H
