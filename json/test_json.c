#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "json.h"

#ifdef NDEBUG
#error "Get out!"
#endif

static void test_roundtrip(const char *input) {
  json_object_t json;
  assert(json_parse(input, &json));

  FILE *tf = tmpfile();
  json_fprint(tf, json);

  int len = ftell(tf);
  fseek(tf, SEEK_SET, 0);

  char *output = (char *)malloc(len + 1);
  fread(output, 1, len, tf);
  fclose(tf);

  if (strcmp(input, output) != 0) {
    fprintf(stderr, "%s !=\n%s\n", output, input);
    exit(1);
  }

  free(output);
}

int main(void) {
  test_roundtrip("{}");
  test_roundtrip("{\"foo\": \"bar\"}");

  test_roundtrip("{\"name\": \"Alice\", \"age\": 30, \"isMember\": true}");
  test_roundtrip("{\"person\": {\"name\": \"Bob\", \"contacts\": {\"email\": "
                 "\"bob@example.com\", \"phone\": null}}}");
  test_roundtrip(
      "{\"employees\": [{\"id\": 1, \"name\": \"John Doe\"}, {\"id\": "
      "2, \"name\": \"Jane Doe\"}]}");
  test_roundtrip("{\"emptyArray\": [], \"emptyObject\": {}}");
  test_roundtrip(
      "{\"mixed\": [123, \"string\", false, null, {\"key\": \"value\"}]}");
  // TODO: Handle quoting in json_fprint().
  // test_roundtrip("{\"text\": \"Hello\\nWorld\\t!\", \"quote\": \"\\\"Double "
  // "Quotes\\\"\"}");
  // test_roundtrip("{\"escaped\": \"Line\\\\nBreak\"}");
  test_roundtrip("{\"largeFloat\": 1.23456e+30}");
  test_roundtrip("{\"truthy\": true, \"falsy\": false}");
  test_roundtrip("{\"level1\": {\"level2\": {\"level3\": {\"level4\": "
                 "{\"key\": \"deep value\"}}}}}");
  test_roundtrip("{\"optional\": null}");
  test_roundtrip("123");
  test_roundtrip("\"string\"");
  test_roundtrip("true");
  test_roundtrip("null");

  printf("all tests passed\n");
  return 0;
}
