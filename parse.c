#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "proto.h"
#include "config.h"

void readnum(char *str, unsigned char *num) {
    /* Generate number from string with binary representation */
    for (int i = 0; i < 8; i++) {
        if (str[i] == '1')
            *num ^= 1 << (7 - i);
    }
}

void parse_qmc(char *arg, unsigned char *loadmem) {
    /* Machine code can be read directly into memory         */
    /* Although this won't do any checking for code validity */
    
    FILE *FD;
    if (!(FD = fopen(arg, "r"))) {
        printf("File %s cannot be opened!\n\n", arg);
        exit(0);
    }
    
    char line[MAXCOL];
    int count = 0;
    while (fgets(line, MAXCOL, FD) != NULL) {
        printf("[Line %d] %s", count, line);
        readnum(line, loadmem + count);
        count++;
    }
    fclose(FD);
}

void parse(char *arg, unsigned char *loadmem) {
    /* Initially zero memory */
    memset(loadmem, 0x0, 256*sizeof(unsigned char));
    
    /* Check file extension to determine if assembly or machine code */
    int len = strlen(arg);
    int i = len;
    while (i > 0) {
        if (arg[i - 1] == '.')
            break;
        i--;
    }
    char ext[len - 1 - i];
    strcpy(ext, arg + i);

    /* Parse based on file extension */
    if (strcmp(ext, "qasm") == 0) {
        printf("Reading assembly code from %s\n", arg);
    } else if (strcmp(ext, "qmc") == 0) {
        printf("Reading machine code from %s\n", arg);
        parse_qmc(arg, loadmem);
    } else {
        printf("Extension of %s not recognised!\n", arg);
        printf("Should be .qasm for assembly, or .qmc for machine code.\n");
        exit(0);
    }
}
