#include <cstdio>
#include <lexer.h>

int main() {
    init_regex();
    char e[] = "!= & | ~ | ! & | + - * / ( ) 0x1234567890\n123\n\n001911\n eqwe";
    Token *tokens = NULL;
    int len = lexer(e, &tokens);
    printf("%d\n", len);
    //Token *t = tokens;
    return 0;
}