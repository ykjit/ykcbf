#include <err.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#define CELLS_LEN 30000

void interp(char *prog, char *prog_end, char *cells, char *cells_end) {
    char *instr = prog;
    char *cell = cells;
    while (instr < prog_end) {
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
                putchar(*cell);
                break;
            }
            case ',': {
                if (read(STDIN_FILENO, cell, 1) == -1)
                    err(1, "(stdin)");
                break;
            }
            case '[': {
                if (*cell == 0) {
                    int count = 0;
                    while (true) {
                        instr++;
                        if (*instr == ']') {
                            if (count == 0)
                                break;
                            count--;
                        } else if (*instr == '[')
                            count++;
                    }
                }
                break;
            }
            case ']': {
                if (*cell != 0) {
                    int count = 0;
                    while (true) {
                        instr--;
                        if (*instr == '[') {
                            if (count == 0)
                                break;
                            count--;
                        } else if (*instr == ']')
                            count++;
                    }
                }
                break;
            }
            default: break;
        }
                instr++;
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

    char *cells = malloc(CELLS_LEN);
    if (cells == NULL)
        err(1, "out of memory");
    for (char *cell = cells; cell < cells + CELLS_LEN; cell++)
        *cell = 0;
    interp(prog, prog + sb.st_size, cells, cells + CELLS_LEN);
}
