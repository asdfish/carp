#include <carp/carp.h>

#include <stdbool.h>
#include <string.h>
#include <sys/types.h>

int carp_parse(int argc, const char** restrict argv, const struct CarpOption* restrict options, size_t options_length, carp_callback callback, void* user_data) {
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

    callback(options[option_index].flag_short, FLAG, i ++ >= argc ? NULL : argv[i], user_data);
    continue;

callback_default:
    callback(0, KEY, argv[i], user_data);
  }
  return 0;
}