#ifndef __LEXER__
#define __LEXER__

#include <stdint.h>

typedef struct token {
  int type;
  char str[32];
  uint64_t val;

  int row;
  int pos;
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
#endif