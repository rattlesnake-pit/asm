#ifndef ASM_H
#define ASM_H

#define TMP_BUFFER_SIZE 100

enum TOKENS {
  PUSH,
  POP,
  COMMENT,
  ENDLINE,
  NAME,
  SYMBOL
};

extern char* str_tokens[];
extern int tokens_size;

void getChar();
int isVarDef(char c);
int isArrayDef(char c);
int isOp();
void next();
void init();
void scan();
void matchString();
void expected(char* s);
int isDefinition(char c);
enum TOKENS token;
char look;
char value[100];
char tmp[TMP_BUFFER_SIZE];
#endif
