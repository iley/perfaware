#ifndef JSON_H_
#define JSON_H_

#include <stdbool.h>
#include <stdio.h>

#include "stb_ds.h"

enum {
  JSON_NUMBER,
  JSON_STRING,
  JSON_BOOLEAN,
  JSON_ARRAY,
  JSON_DICT,
  JSON_NULL,
};

struct json_dict_entry_t;

typedef struct json_object_t {
  int typ;
  union {
    double number;
    char *string; // owned
    bool boolean;
    struct json_dict_entry_t *dict; // owned
    struct json_object_t *array;    // owned
  } val;
} json_object_t;

typedef struct json_dict_entry_t {
  char *key;
  json_object_t value;
} json_dict_entry_t;

json_object_t json_new_number(double value);
json_object_t json_new_string(const char *value);
json_object_t json_new_boolean(bool value);
json_object_t json_new_dict(void);
json_object_t json_new_array(void);
json_object_t json_new_null(void);

void json_free(json_object_t obj);

int json_get_type(json_object_t obj);
bool json_is_number(json_object_t obj);
bool json_is_string(json_object_t obj);
bool json_is_boolean(json_object_t obj);
bool json_is_dict(json_object_t obj);
bool json_is_array(json_object_t obj);
bool json_is_null(json_object_t obj);

double json_get_number(json_object_t obj);
char *json_get_string(json_object_t obj);
bool json_get_boolean(json_object_t obj);

void json_array_append(json_object_t *obj, json_object_t elem);
void json_array_set(json_object_t *obj, int index, json_object_t elem);
int json_array_len(json_object_t obj);
json_object_t json_array_get(json_object_t obj, int index);

void json_dict_set(json_object_t *obj, const char *key, json_object_t value);
json_object_t json_dict_get(json_object_t obj, const char *key);
bool json_dict_has_key(json_object_t obj, const char *key);
int json_dict_len(json_object_t obj);
char *json_dict_get_key(json_object_t obj, int i);

void json_fprint(FILE *out, json_object_t obj);
void json_print(json_object_t obj);

// Returns true if parsed successfully.
bool json_parse(const char *input, json_object_t *output);

#endif // JSON_H_
