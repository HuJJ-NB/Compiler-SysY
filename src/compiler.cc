#include <compiler.h>
#include <lexer.h>
#include <syntax.h>
#include <debug.h>

#include <string.h>
#include <getopt.h>

static char *source = NULL;
static size_t lenth;

static FILE *highlight_fp = NULL;
static bool is_highlight;

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

static inline bool open_highlight_file(char *file) {
  if (highlight_fp) {
    fclose(highlight_fp);
    highlight_fp = NULL;
    Log("Re-open highlight output file");
  }
  if (file) {
    highlight_fp = fopen(file, "w");
    Assert(highlight_fp, "Can not open highlight output file '%s'", file);
  }
  else {
    highlight_fp = stdout;
    Log("highlight output file is not given, output to STDOUT");
  }
  return true;
}

static inline void close_highlight_file() {
  if (highlight_fp) {
    fclose(highlight_fp);
    highlight_fp = NULL;
    Log("Close the highlight output file");
  }
}

static inline bool parse_args(int argc, char *argv[]) {
  const struct option table[] = {
    {"log"          , required_argument     , NULL, 'l'},
    {"highlight"       , required_argument     , NULL, 'h'},
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
      "-h,--highlight=FILE",
      "output highlight to FILE"
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
  while ( (o = getopt_long(argc, argv, "-l:h:", table, NULL)) != -1) {
    switch (o) {
      case 'l'  : log_init(optarg); break;
      case 'h'  : is_highlight = open_highlight_file(optarg); break;
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

int compile(int argc, char *argv[]) {
  is_highlight = false;
  lenth = 0;
  Assert(parse_args(argc, argv), "Source file not given");

  int ret = syntax(source);
  
  close_highlight_file();
  free_src();
  log_free();

  return ret;
}
