#include "lexer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static char *source = NULL;
static size_t lenth;

static inline bool read_src() {
  FILE *fp = fopen("test.c", "r");
  assert(fp);

  fseek(fp, 0, SEEK_END);
  lenth = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  if(source) {
    free(source);
    source = NULL;
  }
  source = (char *)malloc((lenth + 2) * sizeof(char));
  assert(fread(source, sizeof(char), lenth, fp) == lenth);
  if (lenth == 0 || source[lenth - 1] != '\n') source[lenth++] = '\n';
  source[lenth] = '\0';

  fclose(fp);
  return true;
}

static inline void free_src() {
  if (source) {
    free(source);
    source = NULL;
  }
}

void compiler_init(int argc, char *argv[]) {
  lenth = 0;
  read_src();
  lexer_init(source);
}

void compiler_free() {
  free_src();
  lexer_free();
}

static Token *tokens = NULL;

void compile() {
  bool is_EOF = false;
  do{
    Token *t = make_token(&is_EOF);
    if (t == NULL) continue;
    if (tokens == NULL) tokens = t;
  }while (!is_EOF);
  assert(tokens);
  return;
}

int main(int argc, char *argv[]) {
    compiler_init(argc, argv);
    compile();
    compiler_free();
    return 0;
}
