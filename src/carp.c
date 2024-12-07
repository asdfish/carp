#include <carp/carp.h>

#include <stdbool.h>
#include <string.h>

#include <sys/types.h>

bool passed_double_dash = false;

typedef enum {
  LOCAL_SHORT,
  LOCAL_LONG,
  LOCAL_KEY,
} local_argument_t;
int carp_parse(int argc, const char** restrict argv, const carp_option_t* restrict options, size_t options_length, carp_callback callback, void* restrict user_data) {

  for(int i = 1; i < argc; i ++) {
    local_argument_t argument_type = LOCAL_KEY;

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
      result = callback(0, CARP_ARGUMENT_TYPE_KEY, argv[i], user_data);
    } else {
      if(options[option_index].require_argument) {
        i ++;
        result = callback(options[option_index].flag_short, CARP_ARGUMENT_TYPE_FLAG, i >= argc ? NULL : argv[i], user_data);
      } else
        result = callback(options[option_index].flag_short, CARP_ARGUMENT_TYPE_FLAG, argv[i], user_data);
    }

    if(result != 0)
      return result;
  }
  return 0;
}
