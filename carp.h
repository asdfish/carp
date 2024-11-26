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
  enum ArgumentType {
    LOCAL_SHORT,
    LOCAL_LONG,
    LOCAL_KEY,
  };

  for(int i = 1; i < argc; i ++) {
    enum ArgumentType argument_type = LOCAL_KEY;

    if(!passed_double_dash) {
      if(strncmp(argv[i], "--", 2) == 0)
        argument_type = LOCAL_LONG;
      else if(argv[i][0] == '-')
        argument_type = LOCAL_SHORT;
      
      if(strcmp(argv[i], "--") == 0) {
        passed_double_dash = true;
        continue;
      }
    }

    ssize_t option_index = -1;
    for(size_t j = 0; j < options_length; j ++) {
      switch(argument_type) {
        case LOCAL_LONG:
          if(strcmp(options[j].flag_long, argv[i] + 2) == 0) {
            option_index = j;
            goto exit_loop;
          }
          break;
        case LOCAL_SHORT:
          if(options[j].flag_short == argv[i][1]) {
            option_index = j;
            goto exit_loop;
          }
          break;
        case LOCAL_KEY:
          goto exit_loop;
          break;
      }
      continue;

exit_loop:
      break;
    }

    if(argument_type != LOCAL_KEY && option_index == -1)
      argument_type = LOCAL_KEY;

    int result = 0;
    if(argument_type == LOCAL_KEY) {
      result = callback(0, KEY, argv[i], user_data);
    } else {
      if(options[option_index].require_argument) {
        i ++;
        result = callback(options[option_index].flag_short, FLAG, i >= argc ? NULL : argv[i], user_data);
      } else
        result = callback(options[option_index].flag_short, FLAG, argv[i], user_data);
    }

    if(result != 0)
      return result;
  }
  return 0;
}
#endif // CARP_IMPLEMENTATION

#endif // CARP_H
