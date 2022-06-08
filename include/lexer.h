#ifndef __LEXER__
#define __LEXER__

#include <stdint.h>

#define MAX_TOKEN_STR_LEN 512

typedef struct {
  int line_start;
  int line_end;
}Line;

typedef struct {
  int type;
  char str[MAX_TOKEN_STR_LEN];
  uint64_t val;

  int line_no;
  int offset;
  int lenth;
}Token;

/* do lexer_init before this, and lexer_free when all finished. */
/* make tokens and return the point to the token in an array */
/* such as */
/* `Token *tokens;` */
/* `bool is_EOF = false;` */
/* `do{` */
/* `Token *t = make_token(&is_EOF);` */
/* `if (t == NULL) continue;` */
/* `if (tokens == NULL) tokens = t;` */
/* `}while (!is_EOF);` */
/* `tokens` point to the array */
/* Do not free `tokens` */
Token *make_token(bool *is_EOF);

Line get_line_info(int line_no);

void lexer_init(char *src);
void lexer_free();

enum type {
  TK_NOTYPE = 256,
  TK_HEX_INTCON, TK_DEC_INTCON, TK_OCT_INTCON, TK_BIN_INTCON,
  TK_ERR_DEC_INTCON, TK_ERR_OCT_INTCON, TK_ERR_BIN_INTCON,
  TK_ERR_INTCON, //num with not g-zG-Z_
  TK_INTCON,
  TK_ID,
  TK_CONST, TK_INT, TK_VOID, TK_IF, TK_ELSE, TK_WHILE, TK_DO, TK_BREAK, TK_CONTINUE, TK_RETURN,
  TK_SNT, TK_MNT,
  TK_ASSIGN, TK_ADD_ASSIGN, TK_SUB_ASSIGN, TK_MUL_ASSIGN, TK_DIV_ASSIGN, TK_MOD_ASSIGN,
  TK_EQUAL, TK_NEQUAL, TK_LTE, TK_LT, TK_GTE, TK_GT,
  TK_STR,
  TK_EOF,

  /* TODO: Add more token types */
};
#endif