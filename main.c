#include "asm.h"
#include "stdio.h"

int SD = 0; // symbol_def
int PC = 0;
void doVarDef() {
  next();
}
void definitions() {
  while(isDefinition(token)) {
    if(isVarDef(token))
      doVarDef();
    else if(isArrayDef(token)) {

    }
  }
}

void doComment() {
  fprintf(stderr, "DOING COMMENT\n");
  matchString(";");
  while(token != ENDLINE) {
    next();
  }
}

void doLabel() {
  next();
  matchString(":");
  fprintf(stderr, "DOING LABEL\n");
}

void writeByte(char b) {
  outBuffer[PC] = b;
  fprintf(stderr, "wrote byte hex:%x dec:%d\n", b, b);
  PC++;
}

void doOneByteOp() {
  writeByte(token);
  next();
}

void doOp() {
  if(isOneByteOp()) {
    doOneByteOp();
  }
}

void statements() {
  while(look != EOF) {
    if(token == NAME) {
      doLabel();
    }
    if(isOp()) {
      doOp();
    }
    if(token == COMMENT) {
      doComment();
    }
    matchString("\n");
  }
}
void assembler() {
  definitions();
  statements();
}

void emitId() {
  printf("(C)CHUNKUN");
}

void emitDS() {

}

void emitCS() {

}

void emitHeader() {
  emitId();
  emitDS();
  emitCS();
}

void emitCode() {
  for(int i = 0; i < PC; i++) {
    printf("%c", outBuffer[i]);
  }
}

void emitOutput() {
  emitHeader();
  emitCode();
}

int main() {
  init();
  assembler();
  emitOutput();
  return 0;
}
