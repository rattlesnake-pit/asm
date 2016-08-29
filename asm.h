#ifndef ASM_H
#define ASM_H

#define TMP_BUFFER_SIZE 100
#define BYTEOP 0
#define VAROP 1
#define KONSTANTOP 2

#define CHAR 'c'
#define INT 'i'
#define FLOAT 'f'
#define DOUBLE 'd'
#define STRING 's'

#define CHAR_SZ 1
#define INT_SZ 4
#define FLOAT_SZ 4
#define DOUBLE_SZ 8

enum TOKENS {
  HALT,
  PRINTLF,
  PRINT_CHAR,
  PRINT_INT,
  PRINT_FLOAT,
  PRINT_DOUBLE,
  PUSH_CHAR,
  PUSH_INTEGER,
  PUSH_FLOAT,
  PUSH_DOUBLE,
  PUSH_STRING,
  PUSH_ARRAY_CHAR,
  PUSH_ARRAY_INT,
  PUSH_ARRAY_FLOAT,
  PUSH_ARRAY_DOUBLE,
  PUSH_ARRAY_STRING,
  PUSH_CONSTANT_CHAR,
  PUSH_CONSTANT_INT,
  PUSH_CONSTANT_FLOAT,
  PUSH_CONSTANT_DOUBLE,
  PUSH_CONSTANT_STRING,
  POP_CHAR,
  POP_INT,
  POP_FLOAT,
  POP_DOUBLE,
  POP_STRING,
  POP_TO_LOOP_INDEX,
  POP_ARRAY_CHAR,
  POP_ARRAY_INT,
  POP_ARRAY_FLOAT,
  POP_ARRAY_DOUBLE,
  POP_ARRAY_STRING,
  READ_CHAR,
  READ_INT,
  READ_FLOAT,
  READ_DOUBLE,
  READ_STRING,
  READ_ARRAY_CHAR,
  READ_ARRAY_INT,
  READ_ARRAY_FLOAT,
  READ_ARRAY_DOUBLE,
  READ_ARRAY_STRING,
  JUMP,
  JUMP_EQUAL,
  JUMP_NOT_EQUAL,
  JUMP_GREATER,
  JUMP_GREATER_EQUAL,
  JUMP_LOWER,
  JUMP_LOWER_EQUAL,
  STORE_REGISTER,
  STORE_CONSTANT_REGISTER,
  INCREMENT,
  DECREMENT,
  ADD,
  SUBSTRACT,
  MULTIPLY,
  DIVIDE,
  MODULO,
  COMPARE,
  COMMENT,
  ENDLINE,
  DEFINE_CHAR,
  DEFINE_INT,
  DEFINE_FlOAT,
  DEFINE_DOUBLE,
  DEFINE_STRING,
  DEFINE_ARRAY_CHAR,
  DEFINE_ARRAY_INT,
  DEFINE_ARRAY_FLOAT,
  DEFINE_ARRAY_DOUBLE,
  DEFINE_ARRAY_STRING,
  NAME,
  SYMBOL
};

extern char* str_tokens[];
extern int tokens_size;

void getChar();
int isVarDef();
int isArrayDef();
int isOp();
int isOneByteOp();
void next();
void init();
void scan();
void matchString();
void expected(char* s);
void insertSymbol(char type);
void insertLabel();
int isDefinition();
enum TOKENS token;
char look;
char value[TMP_BUFFER_SIZE];
char tmp[TMP_BUFFER_SIZE];
char outBuffer [1000];

struct symbol_row {
  char name[100];
  int start;
  char type;
};

struct label_row {
  char name[100];
  int address;
};

extern int DS; // symbol_def
extern int PC;
struct symbol_row symbol_table[100];
struct label_row label_table[100];
extern int st_end;
extern int lt_end;
#endif
