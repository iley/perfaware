#include "json.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "json_lexer.h"

// JSON data model.

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

json_object_t json_new_dict(void) {
  json_object_t obj = {
      .typ = JSON_DICT,
      .val =
          {
              .dict = NULL,
          },
  };
  return obj;
}

json_object_t json_new_array(void) {
  json_object_t obj = {
      .typ = JSON_ARRAY,
      .val =
          {
              .array = NULL,
          },
  };
  return obj;
};

json_object_t json_new_null(void) {
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
  case JSON_DICT: {
    // We own the key strings, so we need to carefully deallocate them.

    // First, copy all keys into a buffer.
    int len = shlen(obj.val.dict);
    char **keys = (char **)malloc(sizeof(char *) * len);
    for (int i = 0; i < len; i++) {
      keys[i] = obj.val.dict[i].key;
    }

    // Then deallocate the dict.
    shfree(obj.val.dict);

    // Deallocate each key.
    for (int i = 0; i < len; i++) {
      free(keys[i]);
    }

    // Finally, get rid of the buffer we just allocated.
    free(keys);

  } break;
  }
}

int json_get_type(json_object_t obj) { return obj.typ; }

bool json_is_number(json_object_t obj) { return obj.typ == JSON_NUMBER; }
bool json_is_string(json_object_t obj) { return obj.typ == JSON_STRING; }
bool json_is_boolean(json_object_t obj) { return obj.typ == JSON_BOOLEAN; }
bool json_is_dict(json_object_t obj) { return obj.typ == JSON_DICT; }
bool json_is_array(json_object_t obj) { return obj.typ == JSON_ARRAY; }
bool json_is_null(json_object_t obj) { return obj.typ == JSON_NULL; }

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
  char *key_copy = strdup(key);
  shput(obj->val.dict, key_copy, value);
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

char *json_dict_get_key(json_object_t obj, int i) {
  assert(obj.typ == JSON_DICT);
  return obj.val.dict[i].key;
}

// JSON printing

void json_fprint(FILE *out, json_object_t obj) {
  switch (obj.typ) {
  case JSON_NUMBER:
    fprintf(out, "%g", json_get_number(obj));
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
  case JSON_NULL:
    fprintf(out, "null");
    break;
  case JSON_ARRAY: {
    fprintf(out, "[");
    int len = json_array_len(obj);
    for (int i = 0; i < len; i++) {
      json_fprint(out, json_array_get(obj, i));
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
      char *key = json_dict_get_key(obj, i);
      fprintf(out, "\"%s\": ", key);
      json_fprint(out, json_dict_get(obj, key));
      if (i != len - 1) {
        fprintf(out, ", ");
      }
    }
    fprintf(out, "}");
  } break;
  }
}

void json_print(json_object_t obj) { json_fprint(stdout, obj); }

// JSON parsing.

bool json_parse_array(json_lexer_t *lexer, json_object_t *output);
bool json_parse_dict(json_lexer_t *lexer, json_object_t *output);

bool json_parse_value_cont(json_lexer_t *lexer, json_object_t *output) {
  if (lexer->token == JSON_TOK_NUMBER) {
    *output = json_new_number(lexer->numeric_value);
    return true;
  }

  if (lexer->token == JSON_TOK_STRING) {
    *output = json_new_string(lexer->string_value);
    return true;
  }

  if (lexer->token == JSON_TOK_NULL) {
    *output = json_new_null();
    return true;
  }

  if (lexer->token == JSON_TOK_TRUE) {
    *output = json_new_boolean(true);
    return true;
  }

  if (lexer->token == JSON_TOK_FALSE) {
    *output = json_new_boolean(false);
    return true;
  }

  if (lexer->token == '[') {
    return json_parse_array(lexer, output);
  }

  if (lexer->token == '{') {
    return json_parse_dict(lexer, output);
  }

  return true;
}

bool json_parse_value(json_lexer_t *lexer, json_object_t *output) {
  if (!json_lexer_get_token(lexer)) {
    fprintf(stderr, "json error: Unexpected EOF\n");
    return false;
  }

  return json_parse_value_cont(lexer, output);
}

bool json_parse_value_in_array(json_lexer_t *lexer, json_object_t *output,
                               bool *is_array_end) {
  *is_array_end = false;
  if (!json_lexer_get_token(lexer)) {
    fprintf(stderr, "json error: Unexpected EOF\n");
    return false;
  }

  if (lexer->token == ']') {
    *is_array_end = true;
    return false;
  }

  return json_parse_value_cont(lexer, output);
}

bool json_parse_array(json_lexer_t *lexer, json_object_t *output) {
  json_object_t array = json_new_array();

  while (true) {
    json_object_t elem;
    bool is_array_end = false;
    if (!json_parse_value_in_array(lexer, &elem, &is_array_end)) {
      if (is_array_end) {
        break;
      }
      return false;
    }

    json_array_append(&array, elem);

    if (!json_lexer_get_token(lexer)) {
      fprintf(stderr,
              "json error: Unexpected EOF when parsing array seprator\n");
      return false;
    }

    if (lexer->token == ']') {
      // end of the array reached
      break;
    }

    if (lexer->token == ',') {
      continue;
    }

    fprintf(stderr,
            "json error: Unexpected token when parsing array "
            "separator: %d\n",
            lexer->token);
    return false;
  }

  *output = array;
  return true;
}

bool json_parse_dict(json_lexer_t *lexer, json_object_t *output) {
  json_object_t dict = json_new_dict();

  while (true) {
    if (!json_lexer_get_token(lexer)) {
      fprintf(stderr, "json error: Unexpected EOF when parsing dict key\n");
      return false;
    }

    if (lexer->token == '}') {
      // empty dict
      break;
    }

    if (lexer->token != JSON_TOK_STRING) {
      fprintf(stderr,
              "json error: Unexpected token when parsing dict key: %d\n",
              lexer->token);
      return false;
    }

    // Careful! We need to make sure we deallocate this key.
    char *key = strdup(lexer->string_value);

    if (!json_lexer_get_token(lexer)) {
      fprintf(stderr,
              "json error: Unexpected EOF when looking for : in a dict\n");
      free(key);
      return false;
    }

    if (lexer->token != ':') {
      fprintf(stderr,
              "json error: Unexpected token when looking for : in a dict: %d\n",
              lexer->token);
      free(key);
      return false;
    }

    json_object_t value;
    if (!json_parse_value(lexer, &value)) {
      free(key);
      return false;
    }

    json_dict_set(&dict, key, value);
    free(key);

    if (!json_lexer_get_token(lexer)) {
      fprintf(stderr,
              "json error: Unexpected EOF when looking for , or } in a dict\n");
      return false;
    }

    if (lexer->token == '}') {
      break;
    }

    if (lexer->token == ',') {
      continue;
    }

    fprintf(
        stderr,
        "json error: Unexpected token when looking for dict separator: %d\n",
        lexer->token);
    return false;
  }

  *output = dict;
  return true;
}

bool json_parse(const char *input, json_object_t *output) {
  json_lexer_t lexer;
  json_lexer_init(&lexer, input);

  return json_parse_value(&lexer, output);
}
