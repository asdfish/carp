#define CARP_IMPLEMENTATION
#include <carp.h>

#include <stdio.h>

#define ARRAY_LENGTH(array) (sizeof(array) / sizeof(array[0]))

static int callback (char flag_short, enum CarpArgumentType argument_type, const char* argument, void* user_data) {
  printf("flag_short: %c\n", flag_short);
  printf("argument_type: %i\n", argument_type);
  printf("argument: %s\n", argument);
}
static const struct CarpOption options[] = {
  { "output", 'o', true },
  { "verbose", 'v', false },
};

int main(int argc, const char** restrict argv) {
  carp_parse(argc, argv, options, ARRAY_LENGTH(options), callback, NULL);

  return 0;
}
