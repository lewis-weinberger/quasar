#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "globals.h"
#include "proto.h"

void init_emulator(int test, unsigned char *loadmem) {
    /* Initialize system */
    memset(quasar.mem, 0x0, 256*sizeof(unsigned char));
    memset(quasar.R, 0x0, 4*sizeof(unsigned char));
    quasar.IR      = quasar.IAR     = quasar.FLAG = 0x0;
    quasar.address = quasar.value   = 0x0;
    quasar.stop    = 0x1;
    quasar.run     = quasar.single  = 0x0;
    quasar.examine = quasar.deposit = 0x0;
    quasar.quit    = 0x0;
    quasar.rate    = 0x0;
    quasar.rateval = 1;

    /* Blank instruction log */
    for (int i = 0; i < 4; i++) {
        if (i == 3)
            strcpy(quasar.log[i], "READY!        ");
        else
            strcpy(quasar.log[i], "              ");
    }
    
    if (test) {
        /* TEST PROGRAM */
        quasar.mem[0] = 0x20; // DATA R0
        quasar.mem[1] = 0x3A; // (decimal 58)
        quasar.mem[2] = 0x21; // DATA R1
        quasar.mem[3] = 0x0F; // (decimal 15)
        quasar.mem[4] = 0x84; // ADD R1, R0
        quasar.mem[5] = 0x92; // SHR R0, R2
        quasar.mem[6] = 0xA3; // SHL R0, R3
        quasar.mem[7] = 0x40; // JMP
        quasar.mem[8] = 0x00; // (address 0)
    } else {
        memcpy(quasar.mem, loadmem, 256*sizeof(unsigned char));
    }
}

void emulate() {
    unsigned char opcode, RA, RB, addr, cflag, aflag, eflag, zflag;

    /* Check if system is stopped */
    if (quasar.stop == 0) {
        /* Get current instruction pointed to by IAR */
        quasar.IR = quasar.mem[quasar.IAR];

        /* Get high nibble for opcode */
        opcode = quasar.IR >> 4;

        /* Get register operands from low nibble */
        if (opcode >= ADD || opcode <= STORE) {
            RA = (quasar.IR & 0xF) >> 2;
            RB = (quasar.IR & 0x3);
        } else if (opcode == DATA || opcode == JMPR) {
            RB = (quasar.IR & 0x3);
        } else if (opcode == JCAEZ) {
            cflag = (quasar.IR & 0x8) >> 4;
            aflag = (quasar.IR & 0x4) >> 3;
            eflag = (quasar.IR & 0x2) >> 2;
            zflag = (quasar.IR & 0x1);
        }
        
        /* Get next address if needed */
        if (opcode == DATA || opcode == JMP || opcode == JCAEZ) {
            addr = quasar.mem[quasar.IAR+1];
        }
        
        /* Scroll log up by one line */
        memcpy(quasar.log[0], quasar.log[1], 15*sizeof(char));
        memcpy(quasar.log[1], quasar.log[2], 15*sizeof(char));
        memcpy(quasar.log[2], quasar.log[3], 15*sizeof(char));

        /* Perform task according to opcode */
        if (opcode == LOAD) {
            quasar.R[RB] = quasar.mem[quasar.R[RA]];
            sprintf(quasar.log[3], "LOAD R%d R%d    ", RA, RB);
        } else if (opcode == STORE) {
            quasar.mem[quasar.R[RA]] = quasar.R[RB];
            sprintf(quasar.log[3], "STORE R%d R%d   ", RA, RB);
        } else if (opcode == DATA) {
            quasar.R[RB] = addr;
            quasar.IAR++;
            sprintf(quasar.log[3], "DATA R%d 0x%02X  ", RB, addr);
        } else if (opcode == JMPR) {
            quasar.IAR = quasar.R[RB] - 1;
            sprintf(quasar.log[3], "JMPR R%d       ", RB);
        } else if (opcode == JMP) {
            quasar.IAR = addr - 1;
            sprintf(quasar.log[3], "JMP 0x%02X      ", addr);
        } else if (opcode == JCAEZ) {
            if (cflag) {
                if (quasar.FLAG & 0x8)
                    quasar.IAR = addr - 1;
                sprintf(quasar.log[3], "JC 0x%02X      ", addr);
            }
            if (aflag) {
                if (quasar.FLAG & 0x4)
                    quasar.IAR = addr - 1;
                sprintf(quasar.log[3], "JA 0x%02X      ", addr);
            }
            if (eflag) {
                if (quasar.FLAG & 0x2)
                    quasar.IAR = addr - 1;
                sprintf(quasar.log[3], "JE 0x%02X      ", addr);
            }
            if (zflag) {
                if (quasar.FLAG & 0x1)
                    quasar.IAR = addr - 1;
                sprintf(quasar.log[3], "JZ 0x%02X      ", addr);
            }
        } else if (opcode == CLF) {
            quasar.FLAG = 0x0;
            sprintf(quasar.log[3], "CLF           ");
        } else if (opcode == ADD) {
            int acc = quasar.R[RA] + quasar.R[RB];
            if (acc > 0xFF)
                quasar.FLAG = 0x8; /* CARRY flag turned on */
            else if (acc == 0x0)
                quasar.FLAG = 0x1; /* ZERO flag turned on */
            quasar.R[RB] = acc;
            sprintf(quasar.log[3], "ADD R%d R%d     ", RA, RB);
        } else if (opcode == SHR) {
            quasar.R[RB] = quasar.R[RA] >> 1;
            sprintf(quasar.log[3], "SHR R%d R%d     ", RA, RB);
        } else if (opcode == SHL) {
            quasar.R[RB] = quasar.R[RA] << 1;
            sprintf(quasar.log[3], "SHL R%d R%d     ", RA, RB);
        } else if (opcode == NOT) {
            quasar.R[RB] = !(quasar.R[RA]);
            sprintf(quasar.log[3], "NOT R%d R%d     ", RA, RB);
        } else if (opcode == AND) {
            quasar.R[RB] = quasar.R[RA] & quasar.R[RB];
            sprintf(quasar.log[3], "AND R%d R%d     ", RA, RB);
        } else if (opcode == OR) {
            quasar.R[RB] = quasar.R[RA] || quasar.R[RB];
            sprintf(quasar.log[3], "OR R%d R%d      ", RA, RB);
        } else if (opcode == XOR) {
            quasar.R[RB] = quasar.R[RA] ^ quasar.R[RB];
            sprintf(quasar.log[3], "XOR R%d R%d     ", RA, RB);
        } else if (opcode == CMP) {
            if (quasar.R[RA] == quasar.R[RB])
                quasar.FLAG = 0x2; /* EQUAL TO flag turned on */
            else if (quasar.R[RA] > quasar.R[RB])
                quasar.FLAG = 0x4; /* LARGER THAN flag turned on */
            sprintf(quasar.log[3], "CMP R%d R%d     ", RA, RB);
        }

        /* Increment IAR */
        quasar.IAR++;
    }
}
