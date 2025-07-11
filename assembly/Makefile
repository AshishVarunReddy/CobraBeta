%: %.asm
	nasm -f elf64 $< -o $@.o
	ld $@.o -o $@

