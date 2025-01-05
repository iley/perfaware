#include "json_lexer.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#ifdef NDEBUG
#error "Are you ok?"
#endif

static void test_basic_json() {
  json_lexer_t lexer;
  json_lexer_init(&lexer,
                  "{ \"string\": \"some_string_value\", \"number\": 3.14 }");

  assert(json_lexer_get_token(&lexer));
  assert(lexer.token == '{');

  assert(json_lexer_get_token(&lexer));
  assert(lexer.token == JSON_TOK_STRING);
  assert(strcmp(lexer.string_value, "string") == 0);

  assert(json_lexer_get_token(&lexer));
  assert(lexer.token == ':');

  assert(json_lexer_get_token(&lexer));
  assert(lexer.token == JSON_TOK_STRING);
  assert(strcmp(lexer.string_value, "some_string_value") == 0);

  assert(json_lexer_get_token(&lexer));
  assert(lexer.token == ',');

  assert(json_lexer_get_token(&lexer));
  assert(lexer.token == JSON_TOK_STRING);
  assert(strcmp(lexer.string_value, "number") == 0);

  assert(json_lexer_get_token(&lexer));
  assert(lexer.token == ':');

  assert(json_lexer_get_token(&lexer));
  assert(lexer.token == JSON_TOK_NUMBER);
  assert(lexer.numeric_value == 3.14);

  json_lexer_free(&lexer);
}

static void test_string_escaping() {
  json_lexer_t lexer;
  json_lexer_init(&lexer, "\"\\n\\t\\\"\"");

  assert(json_lexer_get_token(&lexer));
  assert(lexer.token == JSON_TOK_STRING);
  assert(strcmp(lexer.string_value, "\n\t\"") == 0);

  json_lexer_free(&lexer);
}

int main() {
  test_basic_json();
  test_string_escaping();
  printf("all tests passed\n");
  return 0;
}
