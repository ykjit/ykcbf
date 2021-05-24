#include <err.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <yk.h>

#define CELLS_LEN 30000

char *jmp_back(char *);
char *jmp_fwd(char *);

void interp(char *prog, char *prog_end, char *cells, char *cells_end, YkLocation *yklocs) {
    // FIXME: need to call yktrace_const on `prog` (or otherwise inform yk that
    // prog is immutable for the duration of a trace).
    YkMT *mt = yk_mt();
    char *instr = prog;
    char *cell = cells;
    while (instr < prog_end) {
        yk_control_point(mt, &yklocs[instr - prog]);
        switch (*instr) {
            case '>': {
                if (cell++ == cells_end)
                    errx(1, "out of memory");
                break;
            }
            case '<': {
                if (cell > cells)
                    cell--;
                break;
            }
            case '+': {
                (*cell)++;
                break;
            }
            case '-': {
                (*cell)--;
                break;
            }
            case '.': {
                if (putchar(*cell) == EOF)
                    err(1, "(stdout)");
                break;
            }
            case ',': {
                if (read(STDIN_FILENO, cell, 1) == -1)
                    err(1, "(stdin)");
                break;
            }
            case '[': {
                if (*cell == 0) {
                    instr = jmp_fwd(instr);
                }
                break;
            }
            case ']': {
                if (*cell != 0) {
                    instr = jmp_back(instr);
                }
                break;
            }
            default: break;
        }
        instr++;
    }
}

// FIXME: needs to be given the yktrace_idempotent attribute
char *jmp_back(char *instr) {
    int count = 0;
    while (true) {
        instr--;
        if (*instr == '[') {
            if (count == 0)
                return instr;
            count--;
        } else if (*instr == ']')
            count++;
    }
}

// FIXME: needs to be given the yktrace_idempotent attribute
char *jmp_fwd(char *instr) {
    int count = 0;
    while (true) {
        instr++;
        if (*instr == ']') {
            if (count == 0)
                return instr;
            count--;
        } else if (*instr == '[')
            count++;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2)
        errx(1, "<filename>");

    int fd = open(argv[1], O_RDONLY);
    struct stat sb;
    if (fstat(fd, &sb) != 0)
        err(1, "%s", argv[1]);
    char *prog = malloc(sb.st_size);
    if (read(fd, prog, sb.st_size) != sb.st_size)
        err(1, "%s", argv[1]);

    char *cells = calloc(1, CELLS_LEN);
    if (cells == NULL)
        err(1, "out of memory");

    YkLocation *yklocs = malloc(sizeof(YkLocation) * CELLS_LEN);
    if (yklocs == NULL)
        err(1, "out of memory");
    for (YkLocation *ykloc = yklocs; ykloc < yklocs + CELLS_LEN; ykloc++)
        *ykloc = yk_location_new();

    interp(prog, prog + sb.st_size, cells, cells + CELLS_LEN, yklocs);
}
