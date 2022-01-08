#include "common.h"

void signal_pipe(int sigo) {
    fputs("client have terminal\n", stdout);
}

void signal_int(int sigo) {
    exit(0);
}