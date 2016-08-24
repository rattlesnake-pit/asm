#include "asm.h"
#include "stdio.h"

int SD = 0; // symbol_def
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
  printf("DOING COMMENT\n");
  matchString(";");
  while(token != ENDLINE) {
    next();
  }
}

void doLabel() {
  next();
  matchString(":");
  printf("DOING LABEL\n");
}


void doOp() {
  switch(token) {
      break;
      break;
    default:
      break;
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

int main() {
  init();
  assembler();
  return 0;
}
