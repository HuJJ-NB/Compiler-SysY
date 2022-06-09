#include <compiler.h>
#include <cstring>

int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        char *input_file = argv[i], *output_file = NULL;
        if (i + 2 < argc) {
            int len = strlen(argv[i + 1]);
            if (len == 2 && strcmp(argv[i + 1], "-o") == 0) {
                output_file = argv[i + 2];
                i += 2;
            }
        }
        if (compile(input_file, output_file)) {
            Log("Compile '%s' success", input_file);
        } else {
            Error("Compile '%s' failed", input_file);
        }
    }
    return 0;
}
