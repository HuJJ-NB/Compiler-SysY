#include <compiler.h>
#include <lexer.h>
#include <debug.h>

#include <stdlib.h>
#include <getopt.h>

static char *source = NULL;

static inline int read_src(char *src) {
  FILE *fp = fopen(src, "r");
  Assert(fp, "Can not open '%s'", src);

  fseek(fp, 0, SEEK_END);
  size_t lenth = ftell(fp);
  Log("The source file is %s, size = %ld", src, lenth); 
  fseek(fp, 0, SEEK_SET);

  if(source) {
    free(source);
    source = NULL;
  }
  source = (char *)malloc(lenth + 1 * sizeof(char));
  Assert(fread(source, sizeof(char), lenth, fp) == lenth, "Can't read all lines in source file");
  source[lenth] = '\0';

  fclose(fp);
  return 0;
}

static int parse_args(int argc, char *argv[]) {
  const struct option table[] = {
    {"log"          , required_argument     , NULL, 'l'},
    {0              , 0                     , NULL, 0},
  };
  int ret = 1;
  int o;
  while ( (o = getopt_long(argc, argv, "-l:", table, NULL)) != -1) {
    switch (o) {
      case 'l'  : log_init(optarg); break;
      case 1    : ret = read_src(optarg);  break;
      default:
        printf("Usage: %s [OPTION...] source [args]\n\n", argv[0]);
        printf("\n");
        panic("Wrong arguments");
    }
  }
  return ret;
}

void compiler_init(int argc, char *argv[]) {
  Assert(parse_args(argc, argv) == 0, "Source file not given.");
  init_regex();
}

int compile() {
  Token *tokens = NULL;
  int len = lexer(source, &tokens);
  Assert(tokens, "error");
  printf("%d tokens get.\n", len);
  //Token *t = tokens;
  return 0;
}