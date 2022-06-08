#include <compiler.h>
#include <debug.h>

int main(int argc, char *argv[]) {
    compiler_init(argc, argv);
    int ret = compile();
    compiler_free();
    return ret;
}
