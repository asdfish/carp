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

typedef int (*carp_callback) (char flag_short, enum CarpArgumentType argument_type, const char* argument, void* user_data);

extern int carp_parse(int argc, const char** restrict argv, const struct CarpOption* restrict options, size_t options_length, carp_callback callback, void* user_data);

#ifdef CARP_IMPLEMENTATION
#include <string.h>
#include <sys/types.h>

int carp_parse(int argc, const char** restrict argv, const struct CarpOption* restrict options, size_t options_length, carp_callback callback, void* user_data) {
  int result = -1;

  for(int i = 1; i < argc; i ++) {
    if(argv[i][0] != '-' || strlen(argv[i]) <= 1)
      goto callback_default;

    bool long_flag = strncmp(argv[i], "--", 2) == 0;

    ssize_t option_index = -1;
    if(long_flag) {
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

    if(option_index == -1)
      goto callback_default;

    if(options[option_index].require_argument) {
      result = callback(options[option_index].flag_short, FLAG, i ++ >= argc ? NULL : argv[i], user_data);
      goto parse_result;
    } else {
      result = callback(options[option_index].flag_short, FLAG, NULL, user_data);
      goto parse_result;
    }

callback_default:
    result = callback(0, KEY, argv[i], user_data);

parse_result:
    if(result != 0)
      return result;
  }
  return 0;
}
#endif // CARP_IMPLEMENTATION

#endif // CARP_H
