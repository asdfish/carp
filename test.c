#include <carp/carp.h>

#include <stdio.h>

#define ARRAY_LENGTH(array) (sizeof(array) / sizeof(array[0]))

static const struct CarpOption options[] = {
  { "output", 'o' },
};
int callback (char flag_short, enum CarpArgumentType argument_type, const char* argument, void* user_data) {
  printf("flag_short: %c\n", flag_short);
  printf("argument_type: ");
  switch(argument_type) {
    case FLAG:
      printf("FLAG\n");
      break;
    case KEY:
      printf("KEY\n");
      break;
    default:
      printf("\n");
      break;
  }
  printf("argument: %s\n", argument);

  return 0;
}

int main(int argc, const char** restrict argv) {
  carp_parse(argc, argv, options, ARRAY_LENGTH(options), callback, NULL);

  return 0;
}
