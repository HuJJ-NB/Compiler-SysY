#ifndef __LEXER__
#define __LEXER__

#include <stdint.h>

typedef struct token {
  int type;
  char str[32];
  uint64_t val;

  int offset;
  int lenth;
}Token;

/* such as */
/* `char expr[] = "int a;";`*/
/* `Token *tokens_p;` */
/* `int N = lexer(expr, &tokens_p);` */
/* make tokens by string `expr` and store in a array */
/* `tokens_p` point to the array */
/* `tokens[0]` to `tokens[N-1]` is koken and `tokens[N]` is just book the finish row and position. */
/* Do not free `tokens_p` */
int lexer(char *src, Token **tokens_p);

/* do this before use lexer */
void init_regex();

void finish_regex();

enum type {
  TK_NOTYPE = 256, TK_ENTER,
  TK_HEX_INTCON, TK_DEC_INTCON, TK_OCT_INTCON, TK_BIN_INTCON,
  TK_ERR_DEC_INTCON, TK_ERR_OCT_INTCON, TK_ERR_BIN_INTCON,
  TK_ERR_INTCON,  //num with not g-zG-Z_
  TK_INTCON,
  TK_DEC_FLOATCON, TK_HEX_FLOATCON, TK_OCT_FLOATCON, TK_ERR_FLOATCON,
  TK_FLOATCON,
  TK_ID,
  TK_CONST, TK_INT, TK_FLOAT, TK_VOID, TK_IF, TK_ELSE, TK_WHILE, TK_DO, TK_BREAK, TK_CONTINUE, TK_RETURN,
  TK_SNT, TK_MNT,
  TK_ASSIGN, TK_ADD_ASSIGN, TK_SUB_ASSIGN, TK_MUL_ASSIGN, TK_DIV_ASSIGN, TK_MOD_ASSIGN,
  TK_EQUAL, TK_NEQUAL, TK_LTE, TK_LT, TK_GTE, TK_GT,
  TK_STR,

  /* TODO: Add more token types */
};
#endif