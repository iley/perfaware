#include <stdio.h>

#include "json.h"

int main() {
  json_object_t obj = json_new_dict();
  json_dict_set(&obj, "foo", json_new_string("bar"));
  json_dict_set(&obj, "baz", json_new_string("qux"));

  json_object_t arr = json_new_array();
  json_array_append(&arr, json_new_number(1));
  json_array_append(&arr, json_new_number(2));
  json_array_append(&arr, json_new_number(3));
  json_array_append(&arr, json_new_boolean(true));

  json_dict_set(&obj, "arr", arr);

  json_print(stdout, obj);
  printf("\n");

  json_free(obj);

  return 0;
}
