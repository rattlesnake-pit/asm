#include "asm.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int PC = 0;
int DS = 0;
int st_end = 0;
int lt_end = 0;
int pending_end = 0;


void doDef(char type, int type_sz) {
  next();
  if(token != NAME) {
    expected("variable name");
  }
  fprintf(stderr, "Doing def %s of size %d \n", value, type_sz);
  insertSymbol(type, type_sz, value);
  next();
}

void doArrDef(char type, int type_sz) {
  next();
  if(token != NAME) {
    expected("variable name");
  }
  char name[100];
  strcpy(name, value);
  next();
  matchString(",");
  if(token != NUMBER) {
    expected("array size");
  }
  int size = atoi(value);

  insertSymbol(type, type_sz * size, name);
  next();
}

void doDefString() {
  next();
  if(token != NAME) {
    expected("variable name");
  }
  char name[100];
  strcpy(name, value);
  next();
  matchString(",");
  if(token != NUMBER) {
    expected("string size");
  }
  int size = atoi(value);
  insertSymbol(STRING,  size, name);
  next();
}

void doDefArrayString() {
  next();
  if(token != NAME) {
    expected("variable name");
  }
  char name[100];
  strcpy(name, value);
  next();
  matchString(",");
  if(token != NUMBER) {
    expected("string size");
  }
  int stringSize = atoi(value);
  next();
  matchString(",");
  if(token != NUMBER) {
    expected("array size");
  }
  int arraySize = atoi(value);
  insertSymbol(STRING,  stringSize * arraySize, name);
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
      doDefString();
      break;
    default:
      break;
  }
}

void doArrayDef() {
  switch(token) {
    case DEFINE_ARRAY_CHAR:
      doArrDef(CHAR, CHAR_SZ);
      break;
    case DEFINE_ARRAY_INT:
      doArrDef(INT, INT_SZ);
      break;
    case DEFINE_ARRAY_FLOAT:
      doArrDef(FLOAT, FLOAT_SZ);
      break;
    case DEFINE_ARRAY_DOUBLE:
      doArrDef(DOUBLE, DOUBLE_SZ);
      break;
    case DEFINE_ARRAY_STRING:
      doDefArrayString();
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
      doArrayDef();
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
  insertLabel();
  fprintf(stderr, "DOING LABEL %s with PC: %d \n", value, PC);
  next();
  matchString(":");
}

void writeByte(unsigned char b) {
  outBuffer[PC] = b;
  fprintf(stderr, "wrote byte hex:%x dec:%d\n", b, b);
  PC++;
}

void writeString(char *val){
    for(size_t i = 0; i < strlen(val); i++){
        writeByte(val[i]);
    }
}

void writeInt(int val){
    for(int i = 24; i >= 0; i -= 8){
        writeByte(val >> i);
    }
    //writeString(intToChar(val));
}

void writeFloat(float val){
    int x = *(int*)&val;
    writeInt(x);
    //writeString(floatToChar(val));
}

void writell(long long val){
    for(int i = 58; i >= 0; i -= 8){
        writeByte(val >> i);
    }
}

void writeDouble(double val){
    long long x = *(long long*)&val;
    writell(x);
    //writeString(doubleToChar(val));
}

void writeSize(char *val){
    int size = strlen(val);
    if(size > 255){
        error("string too long");
    }
    unsigned char len = strlen(val);
    writeByte(len);
}

void writeAddress(int a) {
    writeByte(a>>8);
    writeByte(a);
    fprintf(stderr, "wrote 2 bytes\n");
}

void doOneByteOp() {
  writeByte(token);
  next();
}

void doVarOp(){
    doOneByteOp();
    int address = findVariableAddress(value);
    if(address != -1) {
        writeAddress(address);
        next();
    }
    else{
        error("WE FUCKED UP THERE'S NO ADDRESSS");
    }
}

void addKchar(){
    matchString("'");
    if(strlen(value) != 1){
        expected("it's bigger than it's supposed to");
    }
    if(!isAlphaNum(value[0]))//it returns int but im assuming this works because it returns 0 or 1
    {
        expected("it's neither a number or a letter");
    }
    writeByte(value[0]);
    next();
    matchString("'");
}

void addKint(){
    if(token != NUMBER){
        expected("NaN");
    }
    int intValue = atoi(value);
    writeInt(intValue);
    next();
}

void addKfloat(){
    if(token != NUMBER){
        expected("NaN or float");
    }
    float floatValue = (float)atof(value); //THIS IS NOT FROM STRING TO FLAOT THIS DOES STRING TO DOUBLE FIND THE CORRECT ONE
    writeFloat(floatValue);//TODO: implement writeFloat function
    next();
}

void addKdouble(){
    if(token != NUMBER){
        expected("NaD");
    }
    double doubleValue = atof(value);
    writeDouble(doubleValue);//TODO: implement writeDouble function
    next();
}

void addKstring(){
    matchString("\"");
    if(strlen(value) > 40){
        expected("string is too long");
    }
    if(token != NAME){
        expected("this is not a string");
    }
    writeSize(value);
    writeString(value);
    next();
    matchString("\"");
}

//n'  value = '
void doKonstantOp() {
    switch(token){
        case PUSH_CONSTANT_CHAR:
            doOneByteOp();
            addKchar();
            break;
        case PUSH_CONSTANT_INT:
            doOneByteOp();
            addKint();
            break;
        case PUSH_CONSTANT_FLOAT:
            doOneByteOp();
            addKfloat();
            break;
        case PUSH_CONSTANT_DOUBLE:
            doOneByteOp();
            addKdouble();
            break;
        case PUSH_CONSTANT_STRING:
            doOneByteOp();
            addKstring();
            break;
        case STORE_CONSTANT_REGISTER:
            //maybe make pushcontantint and this one the same
            //I'm not 100% sure it should do the exact same
            doOneByteOp();
            addKint();
            break;
        default:
            expected("WE FUCKED UP IT'S NOT A CONSTANT");
        }
}

void doJump() {
  writeByte(token);
  next();
  if(token != NAME) {
    expected("Variable name");
  }
  int address = findLabelAddress(value);
  if(address == NOT_FOUND) {
    fprintf(stderr, "storing label %s with PC %d to resolve later\n", value, PC);
    insertPendingLabel();
    writeByte(0xFF);
    writeByte(0xFF);
  }
  else {
    fprintf(stderr, "jumping to label %s at address %d\n", value, address);
    writeByte(address>>8);
    writeByte(address);
  }
  next();

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
        case JUMPOP:
            doJump();
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

void resolvePendingLabels() {
  for(int i = 0; i < pending_end; i++) {
    int address = findLabelAddress(pending_label_table[i].name);
    if(address == NOT_FOUND) {
      sprintf(tmp, "LABEL '%s' NOT FOUND", pending_label_table[i].name);
      error(tmp);
    }
    int pc = pending_label_table[i].address;
    fprintf(stderr, "resolved label %s with pc: %d to address %d\n",
        pending_label_table[i].name, pc, address);
    outBuffer[pc] = address >> 8;
    outBuffer[pc+1] = address;
  }
}

int main() {
  init();
  assembler();
  resolvePendingLabels();
  emitOutput();
  return 0;
}
