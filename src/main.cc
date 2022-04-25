#include <cstdio>
#include <common.h>

// static char *file_src = NULL;

void init(int argc, char *argv[]);

int main(int argc, char *argv[]) {
    init(argc, argv);
    char s[] = "!= & | ~ | ! & | + - * / ( ) 0x1234567890\n123\n\n001911\n eqwe";
    Token *tokens = NULL;
    int len = lexer(s, &tokens);
    if (tokens)
        printf("%d tokens get.\n", len);
    else
        printf("error\n");
    //Token *t = tokens;
    return 0;
}

void init(int argc, char *argv[]) {
    //TODO:solve arguments

    init_regex();
}