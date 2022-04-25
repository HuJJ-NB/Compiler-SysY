#include <debug.h>

FILE *log_fp = NULL;

void log_init(const char *log_file) {
  log_fp = NULL;
  if (log_file != NULL) {
    FILE *fp = fopen(log_file, "w");
    Assert(fp, "Can not open '%s'", log_file);
    log_fp = fp;
  }
  if (log_fp) Log("Log is written to %s", log_file);
}