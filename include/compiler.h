#ifndef __COMPILER_H__
#define __COMPILER_H__

void compiler_init(int argc, char *argv[]);
void compiler_free();

// do compiler_init before this, and compiler_free when compile finished.
// return 0 if success, otherwise return 1.
bool compile();

#endif