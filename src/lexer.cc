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
  TK_CONST, TK_INT, TK_FLOAT, TK_VOID, TK_IF, TK_ELSE, TK_WHILE, TK_BREAK, TK_CONTINUE, TK_RETURN,
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
  {"\\s+", TK_NOTYPE},    // spaces
  {"const", TK_CONST},
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
}

void finish_regex() {
  pcre_free(re);
}

static Token tokens[MAX_NR_TOKEN + 1] __attribute__((used)) = {}; //tokens[MAX_NR_TOKEN] is not used as other, just book the finsh position.
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *src) {
  int rule_idx;
  int result[6];

  nr_token = 0;

  int position = 0;
  size_t len = strlen(src);

  while (src[position] != '\0') {
    /* Try all rules one by one. */
    Assert(nr_token < MAX_NR_TOKEN, "Too many tokens.\n");
    for (rule_idx = 0; rule_idx < NR_REGEX; rule_idx++) {
      if (pcre_exec(re[rule_idx], NULL, src, (int)len, position, 0, result, 6) == 1 && result[0] == position) {
        char *substr_start = src + position;
        int substr_len = result[1] - result[0];

        tokens[nr_token].offset = position;
        position += substr_len;

        int type = rules[rule_idx].token_type;

        if(type == TK_NOTYPE) {
          break;
        }

        switch (type) {
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
            tokens[nr_token].type = type;
            break;
        }
        strncpy(tokens[nr_token].str, substr_start, substr_len);

        nr_token++;
        break;
      }
    }

    if (rule_idx == NR_REGEX) {
      printf("No match at %d\n", position);
      printf("%d %d\n", nr_token, tokens[nr_token].offset);
      return false;
    }
  }
  tokens[nr_token].offset = position;
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