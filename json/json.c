#include "json.h"

#include <assert.h>

json_object_t json_new_number(double value) {
  json_object_t obj = {
      .typ = JSON_NUMBER,
      .val =
          {
              .number = value,
          },
  };
  return obj;
}

json_object_t json_new_string(const char *value) {
  char *string_copy = strdup(value);
  json_object_t obj = {
      .typ = JSON_STRING,
      .val =
          {
              .string = string_copy,
          },
  };
  return obj;
}

json_object_t json_new_boolean(bool value) {
  json_object_t obj = {
      .typ = JSON_BOOLEAN,
      .val =
          {
              .boolean = value,
          },
  };
  return obj;
}

json_object_t json_new_dict() {
  json_object_t obj = {
      .typ = JSON_DICT,
      .val =
          {
              .dict = NULL,
          },
  };
  return obj;
}

json_object_t json_new_array() {
  json_object_t obj = {
      .typ = JSON_ARRAY,
      .val =
          {
              .array = NULL,
          },
  };
  return obj;
};

json_object_t json_new_null() {
  json_object_t obj = {
      .typ = JSON_NULL,
  };
  return obj;
}

void json_free(json_object_t obj) {
  switch (obj.typ) {
  case JSON_NUMBER:
    // noop
    break;
  case JSON_STRING: {
    if (obj.val.string != NULL) {
      free(obj.val.string);
    }
  } break;
  case JSON_BOOLEAN:
    // noop
    break;
  case JSON_ARRAY:
    arrfree(obj.val.array);
    break;
  case JSON_DICT:
    shfree(obj.val.dict);
    break;
  }
}

int json_get_type(json_object_t obj) { return obj.typ; }

double json_get_number(json_object_t obj) {
  assert(obj.typ == JSON_NUMBER);
  return obj.val.number;
}

char *json_get_string(json_object_t obj) {
  assert(obj.typ == JSON_STRING);
  return obj.val.string;
}

bool json_get_boolean(json_object_t obj) {
  assert(obj.typ == JSON_BOOLEAN);
  return obj.val.boolean;
}

void json_array_append(json_object_t *obj, json_object_t elem) {
  assert(obj->typ == JSON_ARRAY);
  arrput(obj->val.array, elem);
}

void json_array_set(json_object_t *obj, int index, json_object_t elem) {
  assert(obj->typ == JSON_ARRAY);
  assert(index >= 0 && index < arrlen(obj->val.array));
  obj->val.array[index] = elem;
}

int json_array_len(json_object_t obj) {
  assert(obj.typ == JSON_ARRAY);
  return arrlen(obj.val.array);
}

json_object_t json_array_get(json_object_t obj, int index) {
  assert(obj.typ == JSON_ARRAY);
  assert(index >= 0 && index < arrlen(obj.val.array));
  return obj.val.array[index];
}

void json_dict_set(json_object_t *obj, const char *key, json_object_t value) {
  assert(obj->typ == JSON_DICT);
  shput(obj->val.dict, key, value);
}

json_object_t json_dict_get(json_object_t obj, const char *key) {
  assert(obj.typ == JSON_DICT);
  json_dict_entry_t *entry = shgetp_null(obj.val.dict, key);
  if (entry == NULL) {
    return json_new_null();
  }
  return entry->value;
}

bool json_dict_has_key(json_object_t obj, const char *key) {
  assert(obj.typ == JSON_DICT);
  json_dict_entry_t *entry = shgetp_null(obj.val.dict, key);
  return entry != NULL;
}

int json_dict_len(json_object_t obj) {
  assert(obj.typ == JSON_DICT);
  return shlen(obj.val.dict);
}

const char *json_dict_get_key(json_object_t obj, int i) {
  assert(obj.typ == JSON_DICT);
  return obj.val.dict[i].key;
}

void json_print(FILE *out, json_object_t obj) {
  switch (obj.typ) {
  case JSON_NUMBER:
    fprintf(out, "%lf", json_get_number(obj));
    break;
  case JSON_STRING:
    fprintf(out, "\"%s\"", json_get_string(obj));
    break;
  case JSON_BOOLEAN: {
    bool value = json_get_boolean(obj);
    if (value) {
      fprintf(out, "true");
    } else {
      fprintf(out, "false");
    }
  } break;
  case JSON_ARRAY: {
    fprintf(out, "[");
    int len = json_array_len(obj);
    for (int i = 0; i < len; i++) {
      json_print(out, json_array_get(obj, i));
      if (i != len - 1) {
        fprintf(out, ", ");
      }
    }
    fprintf(out, "]");
  } break;
  case JSON_DICT: {
    fprintf(out, "{");
    int len = json_dict_len(obj);
    for (int i = 0; i < len; i++) {
      const char *key = json_dict_get_key(obj, i);
      fprintf(out, "\"%s\": ", key);
      json_print(out, json_dict_get(obj, key));
      if (i != len - 1) {
        fprintf(out, ", ");
      }
    }
    fprintf(out, "}");
  } break;
  }
}
