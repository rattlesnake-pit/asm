asm: main.c asm.h base.c
	gcc main.c base.c -o asm

test: asm
	./asm < test.rice > CK.chop 2> /dev/null && printf "\nHEX OUTPUT:\n" && xxd CK.chop

dtest: asm
	./asm < test.rice > CK.chop  && printf "\nHEX OUTPUT:\n" && xxd CK.chop
