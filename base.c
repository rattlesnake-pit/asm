#include "stdio.h"
#include "asm.h"
#include "string.h"
#include "stdlib.h"

char* str_tokens[] = {
  "PRTCR",
  "PRTC",
  "PRTI",
  "PRTF",
  "PRTD",
  "PUSHC",
  "PUSHI",
  "PUSHF",
  "PUSHD",
  "PUSHS",
  "PUSHAC",
  "PUSHAI",
  "PUSHAF",
  "PUSHAD",
  "PUSHAS",
  "PUSHKC",
  "PUSHKI",
  "PUSHKF",
  "PUSHKD",
  "PUSHKS",
  "POPC",
  "POPI",
  "POPF",
  "POPD",
  "POPS",
  "POPX",
  "POPAC",
  "POPAI",
  "POPAF",
  "POPAD",
  "POPAS",
  "RDC",
  "RDI",
  "RDF",
  "RDD",
  "RDS",
  "RDAC",
  "RDAI",
  "RDAF",
  "RDAD",
  "RDAS",
  "JMP",
  "JMPEQ",
  "JMPNE",
  "JMPGT",
  "JMPGE",
  "JMPLT",
  "JMPLE",
  "STX",
  "STKX",
  "INC",
  "DEC",
  "ADD",
  "SUB",
  "MUL",
  "DIV",
  "MOD",
  "CMP",
  ";",
  "\n"
};

int tokens_size = sizeof str_tokens / sizeof *str_tokens;

void scan() {
  for(int i = 0; i < tokens_size; i++) {
    if(strcmp(value, str_tokens[i]) == 0) {
      token = (enum TOKENS)i;
    }
  }
}

void getChar() {
  look = getchar();
}

char upcase(char c) {
  if(c>= 'a' && c<= 'z') {
    return ~(1<<5) & c;
  }
  return c;
}

void matchString(char* s) {
  if(strcmp(s, value) != 0) {
    expected(s);
  }
  next();
}

int isAlpha(char c) {
  return upcase(c) >= 'A' && upcase(c) <= 'Z';
}

int isNum(char c) {
  return c >= '0' && c <='9';
}

int isAlphaNum(char c) {
  return isAlpha(c) || isNum(c);
}

void getName() {
  int i = 0;
  while(isAlphaNum(look)) {
    value[i] = upcase(look);
    i++;
    getChar();
  }
  value[i] = '\0';
  token = NAME;
}

void handleWhite() {
  while(look == ' ') getChar();
}
void next() {
  handleWhite();
  if(isAlpha(look)) {
    getName();
  } else {
    value[0] = look;
    value[1] = '\0';
    getChar();
    token = SYMBOL;
  }
  scan();
}

void init() {
  getChar();
  next();
}

int isVarDef(char c) {
  return c == 'v';
}

int isArrayDef(char c) {
  return c == 'a';
}

int isDefinition(char c) {
  return isVarDef(c) || isArrayDef(c);
}

void println(char* s) {
  printf("%s\n", s);
}

void expected(char* s) {
  sprintf(tmp, "EXPECTED '%s'" , s);
  println(tmp);
  exit(1);
}

int isOp() {
  return 0;
}
