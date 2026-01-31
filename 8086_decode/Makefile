EXAMPLES=example1 example2

all: disasm $(EXAMPLES)

disasm: disasm.go
	go build -o $@ $<

$(EXAMPLES): %: %.asm
	nasm $<

run: disasm $(EXAMPLES)
	$(foreach example, $(EXAMPLES), ./disasm $(example);)

clean:
	rm -f disasm $(EXAMPLES)

.PHONY: all clean run
