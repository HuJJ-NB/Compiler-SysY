#ifndef __SYNTAX__
#define __SYNTAX__

#include <lexer.h>
typedef struct SyntaxUnit{
    int type;
    struct SyntaxUnit *first_sub_node;
    struct SyntaxUnit *next;
    Token *token;
} SyntaxUnit;

void syntax_init(char *src);
void syntax_free();
bool syntax();

#endif