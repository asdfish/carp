#ifndef CARP_H
#define CARP_H

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

#ifdef __cplusplus
typedef int (*carp_callback) (char flag_short, enum CarpArgumentType argument_type, const char* argument, void* user_data);
#else
typedef int (*carp_callback) (char flag_short, enum CarpArgumentType argument_type, const char* restrict argument, void* restrict user_data);
#endif

#ifdef __cplusplus
extern int carp_parse(int argc, const char** argv, const struct CarpOption* options, size_t options_length, carp_callback callback, void* user_data);
#else
extern int carp_parse(int argc, const char** restrict argv, const struct CarpOption* restrict options, size_t options_length, carp_callback callback, void* user_data);
#endif

#ifdef CARP_IMPLEMENTATION
#ifdef __cplusplus
#include <cstring>
#else
#include <stdbool.h>
#include <string.h>
#endif

#include <sys/types.h>

bool passed_double_dash = false;

#ifdef __cplusplus
int carp_parse(int argc, const char** argv, const struct CarpOption* options, size_t options_length, carp_callback callback, void* user_data)
#else
int carp_parse(int argc, const char** restrict argv, const struct CarpOption* restrict options, size_t options_length, carp_callback callback, void* user_data)
#endif
{
  int result = -1;

  for(int i = 1; i < argc; i ++) {
    if(argv[i][0] != '-' || strlen(argv[i]) <= 1) {
      result = callback(0, KEY, argv[i], user_data);
      if(result != 0)
        return result;
    }

    bool long_flag = strncmp(argv[i], "--", 2) == 0 && !passed_double_dash;

    ssize_t option_index = -1;
    if(long_flag) {
      if(strcmp(argv[i], "--") == 0) {
        passed_double_dash = true;
        continue;
      }

      char* flag = (char*) argv[i] + 2;
      for(size_t i = 0; i < options_length; i ++)
        if(strcmp(flag, options[i].flag_long) == 0) {
          option_index = i;
          break;
        }
    } else {
      char flag = argv[i][1];
      for(size_t i = 0; i < options_length; i ++)
        if(flag == options[i].flag_short) {
          option_index = i;
          break;
        }
    }

    if(option_index == -1) {
      result = callback(0, KEY, argv[i], user_data);
      if(result != 0)
        return result;
    }

    if(options[option_index].require_argument)
      result = callback(options[option_index].flag_short, FLAG, i ++ >= argc ? NULL : argv[i], user_data);
    else
      result = callback(options[option_index].flag_short, FLAG, NULL, user_data);
    if(result != 0)
      return result;
  }
  return 0;
}
#endif // CARP_IMPLEMENTATION

#endif // CARP_H
