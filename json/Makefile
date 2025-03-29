CC=cc
CFLAGS=-Wall -Wextra -std=gnu17 -g
LIBS=-lm

OBJS=stb_ds.o json.o main.o harvestine.o json_lexer.o timer.o

main: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(OBJS): %.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(filter-out main.o, $(OBJS)): %.o: %.h

main.c: json.h harvestine.h
json.h: stb_ds.h
json.c: json_lexer.h

TESTS=test_lexer test_json

$(TESTS): %: %.c $(filter-out main.o, $(OBJS))
	$(CC) $(CFLAGS) -o $@ $^

test: $(TESTS)
	./test_lexer
	./test_json

clean:
	rm -f main $(OBJS) $(TESTS)

.PHONY: clean test always
