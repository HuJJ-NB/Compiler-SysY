/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <lexer.h>
#include <debug.h>

#include <regex.h>
#include <stdlib.h>
#include <string.h>

#define ARRLEN(arr) (int)(sizeof(arr) / sizeof(arr[0]))
#define MAX_NR_TOKEN 70000
#define MAX_LINE 1000
#define MAX_LINE_LEN 500

enum {
  TK_NOTYPE = 256, TK_ENTER,
  TK_DEC_INTCON, TK_HEX_INTCON, TK_OCT_INTCON,TK_ERR_INTCON,TK_INTCON,
  TK_DEC_FLOATCON, TK_HEX_FLOATCON, TK_OCT_FLOATCON, TK_ERR_FLOATCON, TK_FLOATCON,
  TK_ID,
  TK_INT, TK_VOID, TK_IF, TK_ELSE, TK_WHILE, TK_BREAK, TK_CONTINUE, TK_RETURN,
  TK_SNT, TK_MNT,
  TK_ASSIGN,
  TK_EQUAL, TK_NEQUAL,

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
  {"[a-zA-Z][a-zA-Z0-9_]*", TK_ID},
  {"\\{", '{'},
  {"\\}", '}'},
  {"\\[", '['},
  {"\\]", ']'},
  {"\\(", '('},
  {"\\)", ')'},
  {";", ';'},
  {"//[^\n]*", TK_SNT},
  {"/\\*.*\\*/", TK_MNT},
  {"==", TK_EQUAL},
  {"!=", TK_NEQUAL},
  {"=", TK_ASSIGN},
  {"\\+", '+'},
  {"-", '-'},
  {"\\*", '*'},
  {"/", '/'},
  // {"true", 'T'},
  // {"false", 'F'},
  // {"<", '<'},
  // {">", '>'},
  // {"~|!", '~'},
  // {"&&|&", '&'},
  // {"\\|\\||\\|", '|'},
  {"00[1-9a-fA-F]", TK_ERR_INTCON},
  {"0[1-7][0-7]*", TK_OCT_INTCON},
  {"0x[0-9A-Fa-f]+", TK_HEX_INTCON},
  {"[1-9][0-9]*|0+", TK_DEC_INTCON},
};

const char *regex_display(int type) {
  switch (type) {
  case TK_ENTER:
    return "\\n";
  case TK_SNT:
    return "//[^\\n]*";
  default:
    return rules[type].regex;
  }
}

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

static Token tokens[MAX_NR_TOKEN + 1] __attribute__((used)) = {}; //tokens[MAX_NR_TOKEN] is not used as other, just book the finsh position.
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *src) {
  int rule_idx;
  regmatch_t pmatch;

  nr_token = 0;

  int pos = 0;
  int row = 0;
  int lines[MAX_LINE] = {0};
  int position = 0;

  while (src[position] != '\0') {
    /* Try all rules one by one. */
    Assert(nr_token < MAX_NR_TOKEN, "Too many tokens.\n");
    for (rule_idx = 0; rule_idx < NR_REGEX; rule_idx++) {
      if (regexec(&re[rule_idx], src + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = src + position;
        int substr_len = pmatch.rm_eo;

        /*  if match wrong number  */
        if(rules[rule_idx].token_type == TK_ERR_INTCON) {
          printf("Match wrong number at row %d pos %d\n%s\n%*.s^\n", row, pos, strtok(src + lines[row], "\n"), pos, "");
          return false;
        }

        tokens[nr_token].row = row;
        tokens[nr_token].pos = pos;
        position += substr_len;

        if (rules[rule_idx].token_type == TK_ENTER) {
          lines[++row] = position;
          pos = 0;
          break;
        }
        pos += substr_len;
        if (rules[rule_idx].token_type == TK_NOTYPE) {
          break;
        }

        printf(ASNI_FMT("No.%d match rules[%d] = %s\n", ASNI_FG_BLUE) "%.*s" ASNI_FMT("%.*s", ASNI_FG_YELLOW ) "\n", nr_token, rule_idx, regex_display(rule_idx), tokens[nr_token].pos, src + lines[row], substr_len, substr_start);


        /*  NUM  */
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

        /*  str  */
        strncpy(tokens[nr_token].str, substr_start, substr_len);

        nr_token++;
        break;
      }
    }

    if (rule_idx == NR_REGEX) {
      printf("No match at row %d pos %d\n%s\n%*.s^\n", row, pos, strtok(src + lines[row], "\n"), pos, "");
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