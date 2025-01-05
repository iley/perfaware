#include "json_lexer.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

void json_lexer_init(json_lexer_t *lexer, const char *input) {
  lexer->input = input;
  lexer->numeric_value = 0;
  lexer->string_value = (char *)malloc(JSON_LEXER_MAX_STRING + 1);
}

void json_lexer_free(json_lexer_t *lexer) { free(lexer->string_value); }

static bool is_whitespace(char ch) {
  return ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t';
}

static void skip_whitespace(json_lexer_t *lexer) {
  while (lexer->input[0] && is_whitespace(lexer->input[0])) {
    lexer->input++;
  }
}

static char unescape(char ch) {
  switch (ch) {
  case 'n':
    return '\n';
  case 'r':
    return '\r';
  case 'b':
    return '\b';
  case 'f':
    return '\f';
  case 't':
    return '\t';
  default:
    return ch;
  }
}

static bool is_float_char(char ch) {
  return ('0' <= ch && ch <= '9') || ch == '-' || ch == '.' || ch == 'e' ||
         ch == '+';
}

bool json_lexer_get_token(json_lexer_t *lexer) {
  skip_whitespace(lexer);

  int ch = lexer->input[0];

  if (ch == '\0') {
    return false;
  } else if (ch == '"') {
    lexer->token = JSON_TOK_STRING;
    lexer->input++;

    int len = 0;
    lexer->string_value[0] = '\0';

    while (true) {
      ch = lexer->input[0];
      if (ch == '\0') {
        break;
      }

      if (ch == '"') {
        lexer->input++;
        break;
      }

      if (ch == '\\') {
        lexer->input++;

        ch = lexer->input[0];
        if (ch == '\0') {
          break;
        }

        ch = unescape(ch);
      }

      lexer->string_value[len] = ch;
      lexer->string_value[len + 1] = '\0';
      len++;

      lexer->input++;
    }
  } else if (isdigit(ch) || ch == '-') {
    lexer->token = JSON_TOK_NUMBER;
    int len = 0;
    while (is_float_char(ch)) {
      lexer->string_value[len++] = ch;
      lexer->input++;
      ch = lexer->input[0];
    }
    lexer->numeric_value = atof(lexer->string_value);
  } else if (strncmp(lexer->input, "true", 4) == 0) {
    lexer->token = JSON_TOK_TRUE;
    lexer->input += 4;
  } else if (strncmp(lexer->input, "false", 5) == 0) {
    lexer->token = JSON_TOK_FALSE;
    lexer->input += 5;
  } else if (strncmp(lexer->input, "null", 4) == 0) {
    lexer->token = JSON_TOK_NULL;
    lexer->input += 4;
  } else {
    lexer->token = ch;
    lexer->input++;
  }

  return true;
}
