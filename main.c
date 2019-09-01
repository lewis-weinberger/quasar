/******************************************************************************/
/* QUASAR -- QUite A Small ARchitecture                                       */
/* A basic computer architecture emulation, with parser for assembly code and */
/* console for interacting with system.                                       */
/*                                                                            */
/******************************************************************************/

#include <stdio.h>
#include <string.h>

#include "globals.h" /* Global variables    */
#include "proto.h"   /* Function prototypes */
#include "config.h"  /* Configuration       */

int main(int argc, char **argv) {
    int test = 0;
    unsigned char loadmem[256];

    /* Parse runtime arguments */
    if (argc == 2) {
        if (strcmp(argv[1], "--test") == 0)
            test = 1;
        else
            parse(argv[1], loadmem);
    } else if (argc > 2) {
        printf("Usage: ./quasar [OPTION] / [FILE]\n");
        printf("where the executable takes one argument, either [OPTION] or ");
        printf("[FILE].\n\n");
        printf("[OPTION]\n");
        printf("\t--test: loads a simple test program into memory.\n");
        printf("[FILE]\n");
        printf("\te.g. example.qasm: the path to a script written in assembly to ");
        printf("be loaded into memory.\n");
        return 1;
    } else {
        memset(loadmem, 0x0, 256*sizeof(unsigned char));
    }

    /* Initialize system */
    init_emulator(test, loadmem);
    init_display();

    /* Execution loop */
    int loop_count = 0;
    int rate;
    while (quasar.quit == 0x0) {
        display();
        if (loop_count == 0 || quasar.single)
            emulate();
        rate = DELAY_LOOP/quasar.rateval;
        loop_count = (loop_count + 1) % rate;
    }

    end_display();

    return 0;
}
