#ifndef JSON_LEXER_H_
#define JSON_LEXER_H_

#include <stdbool.h>

#define JSON_LEXER_MAX_STRING 1023

enum {
  JSON_TOK_STRING,
  JSON_TOK_NULL,
  JSON_TOK_TRUE,
  JSON_TOK_FALSE,
  JSON_TOK_NUMBER,
};

typedef struct {
  const char *input;
  int token;
  double numeric_value;
  char *string_value;
} json_lexer_t;

void json_lexer_init(json_lexer_t *lexer, const char *input);
void json_lexer_free(json_lexer_t *lexer);
bool json_lexer_get_token(json_lexer_t *lexer);

#endif // JSON_LEXER_H_
