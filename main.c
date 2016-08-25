#include "asm.h"
#include "stdio.h"

int PC = 0;
int DS = 0;
int st_end = 0;


void doDef(char type, int type_sz) {
  next();
  if(token != NAME) {
    expected("variable name");
  }
  fprintf(stderr, "Doing def %s of size %d \n", value, type_sz);
  insertSymbol(type);
  DS += type_sz;
  next();
}

void doVarDef() {
  switch(token) {
    case DEFINE_CHAR:
      doDef(CHAR, CHAR_SZ);
      break;
    case DEFINE_INT:
      doDef(INT, INT_SZ);
      break;
    case DEFINE_FlOAT:
      doDef(FLOAT, FLOAT_SZ);
      break;
    case DEFINE_DOUBLE:
      doDef(DOUBLE, DOUBLE_SZ);
      break;
    case DEFINE_STRING:
      break;
    default:
      break;
  }
}

void definitions() {
  while(isDefinition(token)) {
    if(isVarDef(token))
      doVarDef();
    else if(isArrayDef(token)) {

    }
    matchString("\n");
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

void doVarOp(){
    //TODO: implement doOp with variables
}

void doKonstantOp(){
    //TODO: implement doOp with constants
}

void statements() {
  while(look != EOF) {
    if(token == NAME) {
      doLabel();
    }
    switch (isOp()) {
        case BYTEOP:
            doOneByteOp();
            break;
        case VAROP:
            doVarOp();
            break;
        case KONSTANTOP:
            doKonstantOp();
            break;
        default:
            break;
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
  fprintf(stderr, "Data segment size: %d\n", DS);
  printf("%c%c",DS>>8,DS);
}

void emitCS() {
  fprintf(stderr, "Code segment size: %d\n", PC);
  printf("%c%c",PC>>8,PC);
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
