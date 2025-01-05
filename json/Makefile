CC=cc
CFLAGS=-Wall -Wextra -std=gnu17 -g
LIBS=-lm

OBJS=stb_ds.o json.o main.o harvestine.o json_lexer.o

main: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(OBJS): %.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(filter-out main.o, $(OBJS)): %.o: %.h

main.c: json.h harvestine.h
json.h: stb_ds.h
json.c: json_lexer.h

test_lexer: test_lexer.o json_lexer.o
	$(CC) $(CFLAGS) -o $@ $^

test: test_lexer
	./test_lexer

clean:
	rm -f main $(OBJS)
