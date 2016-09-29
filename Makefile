asm_lin: main.c asm.h base.c
	gcc main.c base.c -o asm_lin

test: asm_lin
	./asm_lin < test.rice > CK.chop 2> /dev/null && printf "\nHEX OUTPUT:\n" && xxd CK.chop

dtest: asm_lin
	./asm_lin < test.rice > CK.chop  && printf "\nHEX OUTPUT:\n" && xxd CK.chop
asm_win: main.c asm.h base.c
	i686-w64-mingw32-gcc base.c main.c -o asm_win
windtest: asm_win
	wine ./asm_win < test.rice > CKWIN.chop && printf "\nHEX OUTPUT:\n" && xxd CKWIN.chop


