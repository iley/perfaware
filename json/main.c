#include <stdio.h>
#include <stdlib.h>

#include "json.h"

char *slurp(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    return NULL;
  }

  fseek(file, 0, SEEK_END);
  long length = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *buffer = (char *)malloc(length + 1);
  fread(buffer, length, 1, file);

  fclose(file);
  return buffer;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s FILE\n", argv[0]);
    return 1;
  }

  const char *filename = argv[1];

  char *input = slurp(filename);
  json_object_t obj = json_new_dict();

  if (!json_parse(input, &obj)) {
    fprintf(stderr, "Could not parse JSON file %s\n", filename);
    goto exit;
  }

  json_print(obj);
  printf("\n");

exit:
  json_free(obj);
  free(input);

  return 0;
}
