#include <compiler.h>
#include <syntax.h>
#include <debug.h>

#include <string.h>

static char *source = NULL;
static size_t lenth;

static inline bool read_src(char *src) {
  FILE *fp = fopen(src, "r");
  Assert(fp, "Can not open '%s'", src);

  fseek(fp, 0, SEEK_END);
  lenth = ftell(fp);
  Log("The source file is '%s', size = %ld", src, lenth); 
  fseek(fp, 0, SEEK_SET);

  if(source) {
    free(source);
    source = NULL;
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
  }
}

bool compile(char *input, char *output) {
  lenth = 0;
  if (!read_src(input)) return false;
  set_output(output);

  bool ret = syntax(source);

  free_src();
  close_output();

  return ret;
}
