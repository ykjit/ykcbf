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

void interp(char *prog, char *prog_end, char *cells, char *cells_end,
    YkMT *mt, YkLocation *yklocs)
{
    // FIXME: need to call yktrace_const on `prog` (or otherwise inform yk that
    // prog is immutable for the duration of a trace).
    char *instr = prog;
    char *cell = cells;
    while (instr < prog_end) {
        YkLocation *loc = NULL;
        if (*instr == ']')
            loc = &yklocs[instr - prog];
        yk_mt_control_point(mt, loc);
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

    size_t prog_len = sb.st_size;
    char *prog = malloc(prog_len);
    if (prog == NULL)
        err(1, "out of memory");

    if (read(fd, prog, prog_len) != prog_len)
        err(1, "%s", argv[1]);

    char *cells = calloc(1, CELLS_LEN);
    if (cells == NULL)
        err(1, "out of memory");

    YkMT *mt = yk_mt_new();
    YkLocation *yklocs = calloc(prog_len, sizeof(YkLocation));
    if (yklocs == NULL)
        err(1, "out of memory");
    for (YkLocation *ykloc = yklocs; ykloc < yklocs + prog_len; ykloc++)
        *ykloc = yk_location_new();

    interp(prog, prog + prog_len, cells, cells + CELLS_LEN, mt, yklocs);
    free(yklocs);
}
