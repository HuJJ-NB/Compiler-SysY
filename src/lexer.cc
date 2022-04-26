/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <lexer.h>
#include <debug.h>

#include <pcre.h>
#include <stdlib.h>
#include <string.h>

#define ARRLEN(arr) (int)(sizeof(arr) / sizeof(arr[0]))
#define MAX_NR_TOKEN 70000
#define MAX_NR_LINE 1000

enum {
  TK_NOTYPE = 256, TK_ENTER,
  TK_HEX_INTCON, TK_DEC_INTCON, TK_OCT_INTCON, TK_BIN_INTCON,
  TK_ERR_DEC_INTCON, TK_ERR_OCT_INTCON, TK_ERR_BIN_INTCON, TK_ERR_INTCON,
  TK_INTCON,
  TK_DEC_FLOATCON, TK_HEX_FLOATCON, TK_OCT_FLOATCON, TK_ERR_FLOATCON, TK_FLOATCON,
  TK_ID,
  TK_INT, TK_VOID, TK_IF, TK_ELSE, TK_WHILE, TK_BREAK, TK_CONTINUE, TK_RETURN,
  TK_SNT, TK_MNT,
  TK_ASSIGN, TK_ADD_ASSIGN, TK_SUB_ASSIGN, TK_MUL_ASSIGN, TK_DIV_ASSIGN, TK_MOD_ASSIGN,
  TK_EQUAL, TK_NEQUAL, TK_LTE, TK_LT, TK_GTE, TK_GT,
  TK_STR,

  /* TODO: Add more token types */
};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {
  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */
  {"\n", TK_ENTER},
  {"\\s+", TK_NOTYPE},    // spaces
  {"int", TK_INT},
  {"void", TK_VOID},
  {"if", TK_IF},
  {"else", TK_ELSE},
  {"while", TK_WHILE},
  {"break", TK_BREAK},
  {"continue", TK_CONTINUE},
  {"return", TK_RETURN},
  {"[a-zA-Z]\\w*", TK_ID},
  {"\\{", '{'},
  {"\\}", '}'},
  {"\\[", '['},
  {"\\]", ']'},
  {"\\(", '('},
  {"\\)", ')'},
  {";", ';'},
  {",", ','},
  {"//.*", TK_SNT},
  {"/\\*[\\s\\S]*?\\*/", TK_MNT},
  {"\".*?\"", TK_STR},
  {"==", TK_EQUAL},
  {"!=", TK_NEQUAL},
  {"<=", TK_LTE},
  {"<", TK_LT},
  {">=", TK_GTE},
  {">", TK_GT},
  {"!", '!'},
  {"&&|&", '&'},
  {"\\|\\||\\|", '|'},
  {"=", TK_ASSIGN},
  {"\\+=", TK_ADD_ASSIGN},
  {"-=", TK_SUB_ASSIGN},
  {"\\*=", TK_MUL_ASSIGN},
  {"/=", TK_DIV_ASSIGN},
  {"%=", TK_MOD_ASSIGN},
  {"\\+", '+'},
  {"-", '-'},
  {"\\*", '*'},
  {"/", '/'},
  {"%", '%'},
  // {"true", 'T'},
  // {"false", 'F'},
  {"0[xX][\\dA-Fa-f]*[G-Zg-z_][\\w]*", TK_ERR_INTCON},
  {"0[xX][\\dA-Fa-f]+", TK_HEX_INTCON},
  {"0[bB][\\dA-Fa-f]*[G-Zg-z_][\\w]*", TK_ERR_INTCON},
  {"0[bB][01]*[2-9A-Fa-f][\\dA-Fa-f]*", TK_ERR_BIN_INTCON},
  {"0[bB][01]+", TK_BIN_INTCON},
  {"0[\\dA-Fa-f]*[G-Zg-z_][\\w]*", TK_ERR_INTCON},
  {"0[0-7]*[8-9A-Fa-f][\\dA-Fa-f]*", TK_ERR_OCT_INTCON},
  {"0[0-7]+", TK_OCT_INTCON},
  {"[\\dA-Fa-f]*[G-Zg-z_][\\w]*", TK_ERR_INTCON},
  {"\\d+[A-Fa-f][\\dA-Fa-f]*", TK_ERR_DEC_INTCON},
  {"\\d+", TK_DEC_INTCON},
};

const char *regex_display(int type) {
  switch (type) {
  case TK_ENTER:
    return "\\n";
  default:
    return rules[type].regex;
  }
}

#define NR_REGEX ARRLEN(rules)

static pcre *re[NR_REGEX];
static const char *line_rule = ".*\n?";
static pcre *line_re;

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  const char *pcre_error;
  int pcre_error_offset;

  for (i = 0; i < NR_REGEX; i ++) {
    re[i] = pcre_compile(rules[i].regex, 0, &pcre_error, &pcre_error_offset, NULL);
    Assert(re[i], "regex rules[%d] compilation failed at %d: %s\n%s", i, pcre_error_offset, pcre_error, rules[i].regex);
  }
  line_re = pcre_compile(line_rule, 0, &pcre_error, &pcre_error_offset, NULL);
  Assert(line_re, "regex line rules compilation failed at %d: %s\n%s", pcre_error_offset, pcre_error, rules[i].regex);
}

void finish_regex() {
  pcre_free(re);
}

#define put_src_line(fmt, color, ...) \
  do { \
    printf(ASNI_FMT(fmt, color) \
    "%.*s" ASNI_FMT("%.*s", ASNI_FG_YELLOW ) "%.*s\n", \
    __VA_ARGS__, \
    tokens[nr_token].pos, src + lines_start[tokens[nr_token].row], \
    substr_len, substr_start, \
    lines_start[row + 1] - lines_start[tokens[nr_token].row] - tokens[nr_token].pos - substr_len - 1, substr_start + substr_len); \
  } while(0)

static Token tokens[MAX_NR_TOKEN + 1] __attribute__((used)) = {}; //tokens[MAX_NR_TOKEN] is not used as other, just book the finsh position.
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *src) {
  int rule_idx;
  int result[6];

  nr_token = 0;

  int pos = 0;
  int row = 0;

  int position = 0;
  size_t len = strlen(src);

  int lines_start[MAX_NR_LINE + 1];
  int nr_line = 0;
  lines_start[nr_line] = position;
  while (src[lines_start[nr_line]] != '\0') {
    Assert(nr_line < MAX_NR_LINE, "Too many lines (%d)", nr_line);
    Assert(pcre_exec(line_re, NULL, src, (int)len, lines_start[nr_line], 0, result, 6) == 1 && result[0] == lines_start[nr_line], "count line wrong");
    lines_start[++nr_line] = result[1];
    printf("%.*s", lines_start[nr_line] - lines_start[nr_line - 1], src + lines_start[nr_line - 1]);
  }
  printf("\n");
  while (src[position] != '\0') {
    /* Try all rules one by one. */
    Assert(nr_token < MAX_NR_TOKEN, "Too many tokens.\n");
    for (rule_idx = 0; rule_idx < NR_REGEX; rule_idx++) {
      if (pcre_exec(re[rule_idx], NULL, src, (int)len, position, 0, result, 6) == 1 && result[0] == position) {
        char *substr_start = src + position;
        int substr_len = result[1] - result[0];

        tokens[nr_token].row = row;
        tokens[nr_token].pos = pos;

        position += substr_len;

        if (rules[rule_idx].token_type == TK_ENTER) {
          row++;
          pos = 0;
          break;
        }
        else if (rules[rule_idx].token_type == TK_MNT) {
          for (int i = 0; i < substr_len; ++i) {
            if (substr_start[i] == '\n') {row++; pos = 0;}
            else {pos++;}
          }
        }
        else {
          pos += substr_len;
          if (rules[rule_idx].token_type == TK_NOTYPE) {
            break;
          }
        }

        switch (rules[rule_idx].token_type) {
          case TK_DEC_INTCON:
            tokens[nr_token].val = strtoul(substr_start, NULL, 10);
            tokens[nr_token].type = TK_INTCON;
            break;
          case TK_HEX_INTCON:
            tokens[nr_token].val = strtoul(substr_start, NULL, 16);
            tokens[nr_token].type = TK_INTCON;
            break;
          case TK_OCT_INTCON:
            tokens[nr_token].val = strtoul(substr_start, NULL, 8);
            tokens[nr_token].type = TK_INTCON;
          default: 
            tokens[nr_token].type = rules[rule_idx].token_type;
            break;
        }
        strncpy(tokens[nr_token].str, substr_start, substr_len);

        
        if(rules[rule_idx].token_type == TK_ERR_DEC_INTCON) {
          put_src_line("Match wrong decimal number at row %d pos %d\n", ASNI_FG_RED, tokens[nr_token].row, pos);
        }
        else if(rules[rule_idx].token_type == TK_ERR_OCT_INTCON) {
          put_src_line("Match wrong octal number at row %d pos %d\n", ASNI_FG_RED, tokens[nr_token].row, pos);
        }
        else if(rules[rule_idx].token_type == TK_ERR_BIN_INTCON) {
          put_src_line("Match wrong binary number at row %d pos %d\n", ASNI_FG_RED, tokens[nr_token].row, pos);
        }
        else if(rules[rule_idx].token_type == TK_ERR_INTCON) {
          put_src_line("Match wrong number at row %d pos %d\n", ASNI_FG_RED, tokens[nr_token].row, pos);
        }
        else {
          put_src_line("No.%d match rules[%d] %s at row %d pos %d\n", ASNI_FG_BLUE, nr_token, rule_idx, regex_display(rule_idx), tokens[nr_token].row, tokens[nr_token].pos);
          nr_token++;
        }

        break;
      }
    }

    if (rule_idx == NR_REGEX) {
      printf("No match at row %d pos %d\n%s\n%*.s^\n", row, pos, strtok(src + lines_start[row], "\n"), pos, "");
      return false;
    }
  }
  tokens[nr_token].pos = pos;
  tokens[nr_token].row = row;

  return true;
}

int lexer(char *src, Token **tokens_p) {
  Assert(src, "Lexer get no source, points to the zero page.");
  if (make_token(src) == false) {
    *tokens_p = NULL;
    return 0;
  }
  else {
    *tokens_p = tokens;
    return nr_token;
  }
}