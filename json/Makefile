CC=cc
CFLAGS=-Wall -Wextra -std=gnu17 -DDEBUG -g
LIBS=-lm

OBJS=stb_ds.o json.o main.o stb_c_lexer.o harvestine.o

main: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(OBJS): %.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(filter-out main.o, $(OBJS)): %.o: %.h

main.c: json.h harvestine.h
json.h: stb_ds.h
json.c: stb_c_lexer.h

clean:
	rm -f main $(OBJS)
