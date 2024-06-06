#include "hope.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

enum Hopium parse_hope(const char *hope) {
    if (strcmp(hope, "1") == 0) {
        return M_PNUMS;
    }
    else if (strcmp(hope, "2") == 0) {
        return M_PCHANGES;
    }
    else if (strcmp(hope, "3") == 0) {
        return M_EXIT;
    }

    fprintf(stderr, "Unknown signal type: %s\n", hope);
    exit(1);
}