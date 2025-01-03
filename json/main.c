#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "harvestine.h"
#include "json.h"

typedef struct {
  double x0;
  double y0;
  double x1;
  double y1;
} coordinate_pair_t;

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

bool load_input(const char *filename, coordinate_pair_t **out_pairs,
                int *out_pairs_len) {
  bool success = false;
  char *input = slurp(filename);
  json_object_t obj = json_new_dict();

  if (!json_parse(input, &obj)) {
    fprintf(stderr, "Could not parse JSON file %s\n", filename);
    goto exit;
  }

  if (!json_dict_has_key(obj, "pairs")) {
    fprintf(stderr, "load error: \"pairs\" not found\n");
    goto exit;
  }

  json_object_t pairs = json_dict_get(obj, "pairs");
  if (!json_is_array(pairs)) {
    fprintf(stderr, "load error: \"pairs\" expected to be an array\n");
    goto exit;
  }

  *out_pairs_len = json_array_len(pairs);
  *out_pairs =
      (coordinate_pair_t *)malloc(sizeof(coordinate_pair_t) * (*out_pairs_len));

  for (int i = 0; i < *out_pairs_len; i++) {
    json_object_t pair = json_array_get(pairs, i);

    if (!json_dict_has_key(pair, "x0") || !json_dict_has_key(pair, "y0") ||
        !json_dict_has_key(pair, "x1") || !json_dict_has_key(pair, "y1")) {
      fprintf(stderr,
              "load error: one of x0, y0, x1, y1 is missing in pair %d\n", i);
      goto exit;
    }

    json_object_t x0 = json_dict_get(pair, "x0");
    json_object_t y0 = json_dict_get(pair, "y0");
    json_object_t x1 = json_dict_get(pair, "x1");
    json_object_t y1 = json_dict_get(pair, "y1");

    coordinate_pair_t coord_pair = {
        .x0 = json_get_number(x0),
        .y0 = json_get_number(y0),
        .x1 = json_get_number(x1),
        .y1 = json_get_number(y1),
    };

    (*out_pairs)[i] = coord_pair;
  }

  success = true;

exit:
  json_free(obj);
  free(input);
  return success;
}

double average_harvestine(coordinate_pair_t *pairs, int count) {
  double sum = 0;
  for (int i = 0; i < count; i++) {
    sum += reference_haversine(pairs[i].x0, pairs[i].y0, pairs[i].x1,
                               pairs[i].y1, REFERENCE_EARTH_RADIUS);
  }
  return sum / count;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s FILE\n", argv[0]);
    return 1;
  }

  coordinate_pair_t *pairs;
  int npairs;

  const char *filename = argv[1];
  if (!load_input(filename, &pairs, &npairs)) {
    fprintf(stderr, "could not load input from file %s\n", filename);
    return 1;
  }

  printf("loaded %d pairs\n", npairs);

  double answer = average_harvestine(pairs, npairs);
  printf("answer: %lf\n", answer);

  free(pairs);

  return 0;
}
