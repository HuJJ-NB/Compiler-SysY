#include <compiler.h>
#include <lexer.h>
#include <debug.h>

#include <stdlib.h>
#include <string.h>
#include <getopt.h>

static char *source = NULL;
static size_t lenth;

static FILE *syntax_fp = NULL;
static bool is_syntax;

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
    Log("Re-load source");
  }
  source = (char *)malloc((lenth + 2) * sizeof(char));
  Assert(fread(source, sizeof(char), lenth, fp) == lenth, "Can't read all lines in source file");
  if (lenth == 0 || source[lenth - 1] != '\n') source[lenth++] = '\n';
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

static inline bool open_syntax_file(char *file) {
  if (syntax_fp) {
    fclose(syntax_fp);
    syntax_fp = NULL;
    Log("Re-open syntax output file");
  }
  if (file) {
    syntax_fp = fopen(file, "w");
    Assert(syntax_fp, "Can not open syntax output file '%s'", file);
  }
  else {
    syntax_fp = stdout;
    Log("Syntax output file is not given, output to STDOUT");
  }
  return true;
}

static inline void close_syntax_file() {
  if (syntax_fp) {
    fclose(syntax_fp);
    syntax_fp = NULL;
    Log("Close the syntax output file");
  }
}

static bool parse_args(int argc, char *argv[]) {
  const struct option table[] = {
    {"log"          , required_argument     , NULL, 'l'},
    {"syntax"       , required_argument     , NULL, 's'},
    {0              , 0                     , NULL, 0},
  };
  struct {
    const char *head, *tail;
    size_t h_len, t_len;
  } opt_item[] = {
    {
      "-l,--log=FILE",
      "output log to FILE"
    },{
      "-s,--syntax=FILE",
      "output syntax to FILE"
    }
  };

  size_t max_len = 0;
  for (int i = 0; i < (int)(sizeof(opt_item) / sizeof(opt_item[0])); i++) {
    opt_item[i].h_len = strlen(opt_item[i].head);
    opt_item[i].t_len = strlen(opt_item[i].tail);
    if (max_len < opt_item[i].h_len + opt_item[i].t_len) {
      max_len = opt_item[i].h_len + opt_item[i].t_len;
    }
  }

  bool read_from_file = false;
  int o;
  while ( (o = getopt_long(argc, argv, "-l:s:", table, NULL)) != -1) {
    switch (o) {
      case 'l'  : log_init(optarg); break;
      case 's'  : is_syntax = open_syntax_file(optarg); break;
      case 1    : if (!read_from_file) read_from_file = read_src(optarg); break;
      default:
        printf("Usage: %s [OPTION...] source [args]\n\n", argv[0]);
        for (int i = 0; i < (int)(sizeof(opt_item) / sizeof(opt_item[0])); i++) {
          printf("\t%s%-*s%s\n", opt_item[i].head, (int)max_len + 5 - (int)opt_item[i].h_len - (int)opt_item[i].t_len, "", opt_item[i].tail);
        }
        printf("\n");
        panic("Wrong arguments");
    }
  }
  return read_from_file;
}

void compiler_init(int argc, char *argv[]) {
  is_syntax = false;
  lenth = 0;
  Assert(parse_args(argc, argv), "Source file not given");
  lexer_init();
}

void compiler_free() {
  if (is_syntax) {
    close_syntax_file();
  }
  free_src();
  lexer_free();
  log_free();
}

static Token *tokens = NULL;
static int nr_token;

static inline int comp_lexer() {
  bool is_EOF = false;
  tokens = make_token(source, &is_EOF);
  if (tokens != NULL) ++nr_token;
  do{
    Token *t = make_token(NULL, &is_EOF);
    if (tokens == NULL && t != NULL) tokens = t;
    if (t != NULL) ++nr_token;
  }while (!is_EOF);
  Assert(tokens, "Lexer error");
  Log("Lexer success");
  return 0;
}

static inline void comp_syntax() {
  Log("Syntax output");
  if (nr_token == 0) {
    fprintf(syntax_fp, "%s", source);
    return;
  }
  fprintf(syntax_fp, "%.*s", tokens[0].offset, source);
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
    fprintf(syntax_fp, ASNI_FMT( "%s" , "%s") "%.*s", color, tokens[i].str, (i < nr_token - 1 ? tokens[i + 1].offset : (int)strlen(source)) - tokens[i].offset - tokens[i].lenth, source + tokens[i].offset + tokens[i].lenth);
  }
}

void compile() {
  comp_lexer();
  if (is_syntax) {
    comp_syntax();
  }
  Log("Compile success");
  return;
}
