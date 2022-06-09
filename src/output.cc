#include <debug.h>

FILE * output_file = NULL;

static inline bool open_output(char *output) {
    if (output_file) {
        fclose(output_file);
    }
    if (output == NULL) {
        Log("Output file not specified");
        return false;
    }
    output_file = fopen(output, "w");
    return output_file != NULL;
}

void set_output(char *output) {
    if (open_output(output)) {
        Log("Set output file to '%s'", output);
        return;
    }
    Log("Set output file to stdout");
}

void close_output() {
    Log("Close output file");
    fclose(output_file);
}