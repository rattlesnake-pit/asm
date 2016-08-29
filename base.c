#include "stdio.h"
#include "asm.h"
#include "string.h"
#include "stdlib.h"

char* str_tokens[] = {
  "HALT",
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
  "\n",
  "DEFC",
  "DEFI",
  "DEFF",
  "DEFD",
  "DEFS",
  "DEFAC",
  "DEFAI",
  "DEFAF",
  "DEFAD",
  "DEFAS"
};

int tokens_size = sizeof str_tokens / sizeof *str_tokens;

int findVariableAddress(char *value){
  for(int i = 0; i < st_end; i++){
    if(strcmp(symbol_table[i].name,value) == 0){
      return symbol_table[i].start;
    }
  }
  return -1;
}

void insertPendingLabel() {
  struct label_row row;
  row.address = PC;
  strcpy(row.name, value);
  pending_label_table[pending_end] = row;
  pending_end++;
}

void insertLabel() {
  struct label_row row;
  strcpy(row.name, value);
  row.address = PC;
  label_table[lt_end] = row;
  lt_end++;
}

int findLabelAddress(char* label) {
  for(int i = 0; i < lt_end; i++) {
    if(strcmp(label_table[i].name, label) == 0) {
      return label_table[i].address;
    }
  }
  return NOT_FOUND;
}

void insertSymbol(char type) {
  struct symbol_row row;
  strcpy(row.name, value);
  row.start = DS;
  row.type = type;
  symbol_table[st_end] = row;
  st_end++;
}

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

void getNum(){
    int i = 0;
    while(isNum(look)){
        value[i] = look;
        i++;
        getChar();
    }
    value[i] = '\0';
    token = NUMBER;
}

void handleWhite() {
  while(look == ' ') getChar();
}
void next() {
  handleWhite();
  if(isAlpha(look)) {
    getName();
  }
  else if(isNum(look)) {
    getNum();
  }
  else {
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

int isVarDef() {
  return token >= DEFINE_CHAR && token <= DEFINE_STRING;
}

int isArrayDef() {
  return token >= DEFINE_ARRAY_CHAR && token <= DEFINE_ARRAY_STRING;
}

int isDefinition() {
  return isVarDef() || isArrayDef();
}

void println(char* s) {
  printf("%s\n", s);
}

void error(char* s) {
  println(s);
  exit(1);
}

void expected(char* s) {
  sprintf(tmp, "EXPECTED '%s'" , s);
  error(tmp);
}


int isOneByteOp() {
  return token == HALT ||
         token == PRINTLF ||
         token == POP_TO_LOOP_INDEX ||
        (token >= ADD && token <= COMPARE); //COMPARE checks the values of the 2 places in the stack and adds it to a registry so that means it is a onebyteop as well
}

int isKonstantOp() {
  return token >= PUSH_CONSTANT_CHAR && token <= PUSH_CONSTANT_STRING ||
           token == STORE_CONSTANT_REGISTER;
}

int isVarOp() {
  return token >= PRINT_CHAR && token <= PUSH_ARRAY_STRING ||
         token >= POP_CHAR && token <= READ_ARRAY_STRING ||
         token >= STORE_REGISTER && token <= DECREMENT;
}

int isJump() {
  return token >= JUMP && token <= JUMP_LOWER_EQUAL;
}
int isOp() {
  if(isOneByteOp()) {
    return BYTEOP;
  }
  if(isKonstantOp()) {
    return KONSTANTOP;
  }
  if(isVarOp()) {
    return VAROP;
  }
  if(isJump()) {
    return JUMPOP;
  }
  return -1;
}
