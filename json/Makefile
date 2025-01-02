CC=cc
CFLAGS=-Wall -Wextra -std=gnu17 -DDEBUG -g

main: main.o json.o stb_ds.o stb_c_lexer.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c json.h
	$(CC) $(CFLAGS) -c -o $@ $<

json.o: json.c json.h stb_c_lexer.h stb_ds.h
	$(CC) $(CFLAGS) -c -o $@ $<

stb_ds.o: stb_ds.c stb_ds.h
	$(CC) $(CFLAGS) -c -o $@ $<

stb_c_lexer.o: stb_c_lexer.c stb_c_lexer.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f main.o json.o
