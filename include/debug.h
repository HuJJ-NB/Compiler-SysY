#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdio.h>
#include <assert.h>

// ----------- asni ----------

#define ASNI_FG_BLACK   "\33[1;30m"
#define ASNI_FG_RED     "\33[1;31m"
#define ASNI_FG_GREEN   "\33[1;32m"
#define ASNI_FG_YELLOW  "\33[1;33m"
#define ASNI_FG_BLUE    "\33[1;34m"
#define ASNI_FG_MAGENTA "\33[1;35m"
#define ASNI_FG_CYAN    "\33[1;36m"
#define ASNI_FG_WHITE   "\33[1;37m"
#define ASNI_BG_BLACK   "\33[1;40m"
#define ASNI_BG_RED     "\33[1;41m"
#define ASNI_BG_GREEN   "\33[1;42m"
#define ASNI_BG_YELLOW  "\33[1;43m"
#define ASNI_BG_BLUE    "\33[1;44m"
#define ASNI_BG_MAGENTA "\33[1;35m"
#define ASNI_BG_CYAN    "\33[1;46m"
#define ASNI_BG_WHITE   "\33[1;47m"
#define ASNI_NONE       "\33[0m"

#define ASNI_FMT(str, fmt) fmt str ASNI_NONE

// ----------- log -----------

void log_init(const char *log_file);
void log_free();

#define Log(format, ...) \
  do { \
    printf(ASNI_FMT("[%s:%d %s] " format, ASNI_FG_BLUE) "\n", \
        __FILE__, __LINE__, __func__, ## __VA_ARGS__); \
    log_write("[%s:%d %s] " format "\n", \
        __FILE__, __LINE__, __func__, ## __VA_ARGS__); \
  } while (0)

#define log_write(...) \
  do { \
    extern FILE* log_fp; \
    if (log_fp) { \
      fprintf(log_fp, __VA_ARGS__); \
      fflush(log_fp); \
    } \
  } while (0) \

void set_output(char *output);
void close_output();

#define Error(format, ...) \
  do { \
    fflush(stdout); \
    fprintf(stderr, ASNI_FMT("[%s:%d %s] " format, ASNI_FG_RED) "\n", \
        __FILE__, __LINE__, __func__, ## __VA_ARGS__); \
  } while (0)

#define Print(format, ...) \
  do { \
    extern FILE *output_file; \
    if (output_file) { \
      fprintf(output_file, format, ## __VA_ARGS__); \
    } else { \
      printf(format, ## __VA_ARGS__); \
    } \
  } while (0)

// ---------- Assert ---------

#define Assert(cond, format, ...) \
  do { \
    if (!(cond)) { \
      log_write(format "\n" , ## __VA_ARGS__); \
      fflush(stdout); \
      fprintf(stderr, ASNI_FMT(format, ASNI_FG_RED) "\n", ##  __VA_ARGS__); \
      assert(cond); \
    } \
  } while (0)

#define panic(format, ...) Assert(0, format, ## __VA_ARGS__)

#define TODO() panic("[%s:%d %s] Something havent done please implement it.", __FILE__, __LINE__, __func__);

#endif
