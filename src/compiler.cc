#include <compiler.h>
#include <lexer.h>
#include <debug.h>

#include <stdlib.h>
#include <string.h>
#include <getopt.h>

static char *source = NULL;
static size_t lenth;

static inline bool read_src(char *src) {
  FILE *fp = fopen(src, "r");
  Assert(fp, "Can not open '%s'", src);

  fseek(fp, 0, SEEK_END);
  lenth = ftell(fp);
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
  return true;
}

static inline void free_src() {
  if (source) {
    free(source);
    source = NULL;
    Log("Free the mem of source map");
  }
}

static bool parse_args(int argc, char *argv[]) {
  const struct option table[] = {
    {"log"          , required_argument     , NULL, 'l'},
    {0              , 0                     , NULL, 0},
  };
  bool read_from_file = false;
  int o;
  while ( (o = getopt_long(argc, argv, "-l:s", table, NULL)) != -1) {
    switch (o) {
      case 'l'  : log_init(optarg); break;
      case 1    : read_from_file = read_src(optarg); break;
      default:
        printf("Usage: %s [OPTION...] source [args]\n\n", argv[0]);
        printf("\t-l,--log=FILE           output log to FILE\n");
        printf("\n");
        panic("Wrong arguments");
    }
  }
  return read_from_file;
}

void compiler_init(int argc, char *argv[]) {
  Assert(parse_args(argc, argv), "Source file not given");
  init_regex();
}

void compiler_free() {
  free_src();
  finish_regex();
  log_free();
}

static Token *tokens = NULL;
static int nr_token;

static inline int comp_lex() {
  nr_token = lexer(source, &tokens);
  Assert(tokens, "Lexer error");
  Log("Lexer success");
  return 0;
}

void compile() {
  comp_lex();
  Log("Compile success");
  return;
}

void syntax() {
  comp_lex();

  for (int i = 0; i < nr_token; ++i) {
    const char *color;
    switch (tokens[i].type) {
    case TK_INT: case TK_VOID: case TK_FLOAT:
      color = ASNI_FG_MAGENTA;
      break;
    case TK_CONST:
    case TK_IF: case TK_ELSE:
    case TK_DO: case TK_WHILE: case TK_BREAK: case TK_CONTINUE:
    case TK_RETURN:
      color = ASNI_FG_BLUE;
      break;
    case TK_INTCON:
      color = ASNI_FG_YELLOW;
      break;
    case TK_ID:
      color = ASNI_FG_GREEN;
      break;
    case TK_MNT: case TK_SNT:
      color = ASNI_FG_BLACK;
      break;
    case TK_ERR_BIN_INTCON: case TK_ERR_DEC_INTCON: case TK_ERR_OCT_INTCON: case TK_ERR_INTCON:
    case TK_ERR_FLOATCON:
      color = ASNI_FG_RED;
      break;
    default:
      color = ASNI_FG_WHITE;
      break;
    }
    printf(ASNI_FMT( "%s%.*s" , "%s"), color, tokens[i].str, tokens[i + 1].offset - tokens[i].offset - tokens[i].lenth, source + tokens[i].offset + tokens[i].lenth);
  }
  if (source[lenth - 1] != '\n') printf("\n");
  Log("Syntax output");
}