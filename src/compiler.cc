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

bool print_line(int line_no) {
  Line line_info = get_line_info(line_no);
  if (line_info.line_end < line_info.line_start) return false;

  printf("%.*s", line_info.line_end - line_info.line_start, source + line_info.line_start);
  return true;
}

void compiler_init(int argc, char *argv[]) {
  is_syntax = false;
  lenth = 0;
  Assert(parse_args(argc, argv), "Source file not given");
  lexer_init(source);
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

static inline void comp_lexer() {
  Log("Syntax output");
  bool is_EOF = false;
  int last_token_end = 0;
  do{
    Token *t = make_token(&is_EOF);
    if (t == NULL) continue;
    if (tokens == NULL) tokens = t;
    if (is_syntax) {
      const char *color;
      switch (t->type) {
      case TK_INT: case TK_VOID:
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
        color = ASNI_FG_RED;
        break;
      default:
        color = ASNI_FG_WHITE;
        break;
      }
      fprintf(syntax_fp, "%.*s" ASNI_FMT( "%.*s" , "%s" ), t->offset - last_token_end, source + last_token_end, color, t->lenth, t->str);
      last_token_end = t->offset + t->lenth;
    }
  }while (!is_EOF);
  if (is_syntax) {
    fprintf(syntax_fp, "%s", source + last_token_end);
  }
  Assert(tokens, "Lexer error");
  Log("Lexer success");
}

void compile() {
  comp_lexer();
  Log("Compile success");
  return;
}
