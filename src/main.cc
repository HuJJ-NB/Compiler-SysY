#include <compiler.h>
#include <debug.h>

int main(int argc, char *argv[]) {
    compiler_init(argc, argv);
    compile();
    compiler_free();
    return 0;
}
