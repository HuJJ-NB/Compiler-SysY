#include <compiler.h>
#include <debug.h>

int main(int argc, char *argv[]) {
    compiler_init(argc, argv);
    syntax();
    compile();
    compiler_free();
    return 0;
}
