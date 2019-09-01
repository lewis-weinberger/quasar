#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>

#include "globals.h"
#include "config.h"

void printnum(unsigned char num, char *str) {
    /* Create string with binary, hex and decimal representations of number */
    sprintf(str, "%c%c%c%c %c%c%c%c (0x%02X) (%03d)", 
            num & 0x80 ? '1' : '0', 
            num & 0x40 ? '1' : '0', 
            num & 0x20 ? '1' : '0', 
            num & 0x10 ? '1' : '0', 
            num & 0x08 ? '1' : '0', 
            num & 0x04 ? '1' : '0', 
            num & 0x02 ? '1' : '0', 
            num & 0x01 ? '1' : '0',
            num, num); 
}

void myborder(int left, int top, int right, int bottom) {
    /* Create a border defined by corner coordinates */
    for (int i = left + 1; i < right + 1; i++){
        attron(COLOR_PAIR(3));
        mvprintw(bottom, i, "_");
        mvprintw(top, i, "_");
        attroff(COLOR_PAIR(3));
    }
    for (int i = bottom + 1; i < top + 1; i++){
        attron(COLOR_PAIR(3));
        mvprintw(i, left, "|");
        mvprintw(i, right + 1, "|");
        attroff(COLOR_PAIR(3));
    }
}

void background() {
    /* Generate background console layout */
    attron(COLOR_PAIR(1));
    for (int j = 0; j < NROWS; j++) {
        for (int i = 0; i < NCOLS; i++) {
            mvaddch(j, i, ' ');
        }
    }
    attroff(COLOR_PAIR(1));

    /* Logo ASCII word art */
    int xl, yl;
    xl = 1;
    yl = NROWS - 6;
    attron(COLOR_PAIR(2));
    mvprintw(yl, xl, "  / __ \\         version 1.0.0             ");
    mvprintw(yl + 1, xl, " | |  | |_   _  __ _ ___  __ _ _ __  ");
    mvprintw(yl + 2, xl, " | |  | | | | |/ _` / __|/ _` | '__| ");
    mvprintw(yl + 3, xl, " | |__| | |_| | (_| \\__ \\ (_| | |    ");
    mvprintw(yl + 4, xl, "  \\___\\_\\\\__,_|\\__,_|___/\\__,_|_|    ");
    attroff(COLOR_PAIR(2));
    
    /* Buttons */
    attron(COLOR_PAIR(3));
    mvprintw(quasar.buttony[0], quasar.buttonx[0], "[S]TOP");
    mvprintw(quasar.buttony[1], quasar.buttonx[1], "[R]UN");
    mvprintw(quasar.buttony[2], quasar.buttonx[2], "S[I]NGLE-STEP");
    mvprintw(quasar.buttony[3], quasar.buttonx[3], "[E]XAMINE");
    mvprintw(quasar.buttony[4], quasar.buttonx[4], "[D]EPOSIT");
    mvprintw(quasar.buttony[5], quasar.buttonx[5], "R[A]TE");
    mvprintw(quasar.buttony[6], quasar.buttonx[6], "[Q]UIT");
    attroff(COLOR_PAIR(3));

    /* Button outline */
    myborder(quasar.buttonx[0] - 2, quasar.buttony[2] + 5,
             quasar.buttonx[2] + 14, quasar.buttony[0] - 2);

    /* Log outline */
    myborder(1, quasar.buttony[0] - 2, 17, quasar.buttony[0] - 8);
    
    /* Registers outline */
    myborder(quasar.buttonx[0] - 2, quasar.buttony[0] - 3,
             quasar.buttonx[2] + 14, quasar.buttony[0] - 16);

    /* Register positions */
    attron(COLOR_PAIR(3));
    mvprintw(quasar.buttony[0] - 14, quasar.buttonx[0], "R0");
    mvprintw(quasar.buttony[0] - 14, quasar.buttonx[0] + 10 + NCOLS/10, "R1");
    mvprintw(quasar.buttony[0] - 11, quasar.buttonx[0], "R2");
    mvprintw(quasar.buttony[0] - 11, quasar.buttonx[0] + 10 + NCOLS/10, "R3");
    mvprintw(quasar.buttony[0] - 8, quasar.buttonx[0], "IR");
    mvprintw(quasar.buttony[0] - 8, quasar.buttonx[0] + 10 + NCOLS/10, "IAR");
    mvprintw(quasar.buttony[0] - 5, quasar.buttonx[0], "FLAG");
    attroff(COLOR_PAIR(3));
    
    /* User input outline */
    myborder(1, quasar.buttony[0] - 9, 35, quasar.buttony[0] - 16);

    /* Input positions */
    attron(COLOR_PAIR(3));
    mvprintw(quasar.buttony[0] - 14, 3, "ADDRESS");
    mvprintw(quasar.buttony[0] - 11, 3, "DATA");
    attroff(COLOR_PAIR(3));
    attron(COLOR_PAIR(2));
    char ustr[23];
    printnum(quasar.address, ustr);
    mvprintw(quasar.buttony[0] - 13, 3, ustr);
    printnum(quasar.value, ustr);
    mvprintw(quasar.buttony[0] - 10, 3, ustr);
    attroff(COLOR_PAIR(2));
}

void display() {
    background();

    /* Write register values */
    char regstr[11];
    attron(COLOR_PAIR(2));
    sprintf(regstr, "0x%02X (%03d)", quasar.R[0], quasar.R[0]);
    mvprintw(quasar.buttony[0] - 13, quasar.buttonx[0], regstr);
    sprintf(regstr, "0x%02X (%03d)", quasar.R[1], quasar.R[1]);
    mvprintw(quasar.buttony[0] - 13, quasar.buttonx[0] + 10 + NCOLS/10, regstr);
    sprintf(regstr, "0x%02X (%03d)", quasar.R[2], quasar.R[2]);
    mvprintw(quasar.buttony[0] - 10, quasar.buttonx[0], regstr);
    sprintf(regstr, "0x%02X (%03d)", quasar.R[3], quasar.R[3]);
    mvprintw(quasar.buttony[0] - 10, quasar.buttonx[0] + 10 + NCOLS/10, regstr);
    sprintf(regstr, "0x%02X (%03d)", quasar.IR, quasar.IR);
    mvprintw(quasar.buttony[0] - 7, quasar.buttonx[0], regstr);
    sprintf(regstr, "0x%02X (%03d)", quasar.IAR, quasar.IAR);
    mvprintw(quasar.buttony[0] - 7, quasar.buttonx[0] + 10 + NCOLS/10, regstr);
    sprintf(regstr, "0x%02X (%03d)", quasar.FLAG, quasar.FLAG);
    mvprintw(quasar.buttony[0] - 4, quasar.buttonx[0], regstr);
    attroff(COLOR_PAIR(2));

    /* Write instruction log */
    for (int i = 0; i < 4; i++) {
        attron(COLOR_PAIR(3));
        mvprintw(quasar.buttony[0] - 6 + i, 2, quasar.log[i]);
        attroff(COLOR_PAIR(3));
    }

    /* Highlight active buttons */
    if (quasar.stop == 0x1) {
        attron(COLOR_PAIR(4));
        mvprintw(quasar.buttony[0], quasar.buttonx[0], "[S]TOP");
        attroff(COLOR_PAIR(4));
    } else if (quasar.run == 0x1) {
        attron(COLOR_PAIR(4));
        mvprintw(quasar.buttony[1], quasar.buttonx[1], "[R]UN");
        attroff(COLOR_PAIR(4));
    } else if (quasar.single == 0x1) {
        attron(COLOR_PAIR(4));
        mvprintw(quasar.buttony[2], quasar.buttonx[2], "S[I]NGLE-STEP");
        attroff(COLOR_PAIR(4));
    } 
    
    if (quasar.examine == 0x1) {
        attron(COLOR_PAIR(4));
        mvprintw(quasar.buttony[3], quasar.buttonx[3], "[E]XAMINE");
        attroff(COLOR_PAIR(4));
    } else if (quasar.deposit == 0x1) {
        attron(COLOR_PAIR(4));
        mvprintw(quasar.buttony[4], quasar.buttonx[4], "[D]EPOSIT");
        attroff(COLOR_PAIR(4));
    }
    
    if (quasar.rate == 0x1) {
        attron(COLOR_PAIR(4));
        mvprintw(quasar.buttony[5], quasar.buttonx[5], "R[A]TE");
        attroff(COLOR_PAIR(4));
    }

    /* Process user input for examine/deposit functions */
    if (quasar.examine == 0x1) {
        int input = 1;
        int cpos  = 0;
        /* Allow user to input address value */
        while (input) {
            if (cpos < 4)
                move(quasar.buttony[0] - 13, 3 + cpos);
            else 
                move(quasar.buttony[0] - 13, 4 + cpos);
            
            int ech = getch();
            if (ech == 10 || ech == KEY_ENTER) {
                input = 0;
            } else if (ech == KEY_LEFT) {
                cpos -= 1;
                if (cpos < 0)
                    cpos = 7;
            } else if (ech == KEY_RIGHT) {
                cpos += 1;
                if (cpos > 7)
                    cpos = 0;
            } else if (ech == ' ') {
                quasar.address ^= (1 << (7 - cpos));
            }
            attron(COLOR_PAIR(2) | A_STANDOUT);
            char ustr[23];
            printnum(quasar.address, ustr);
            mvprintw(quasar.buttony[0] - 13, 3, ustr);
            attroff(COLOR_PAIR(2) | A_STANDOUT);
        }
        quasar.value = quasar.mem[quasar.address];
        quasar.examine = 0x0;
    } else if (quasar.deposit == 0x1) {
        int input = 1;
        int cpos  = 0;
        /* Allow user to input address value */
        while (input) {
            if (cpos < 4)
                move(quasar.buttony[0] - 13, 3 + cpos);
            else 
                move(quasar.buttony[0] - 13, 4 + cpos);
            
            int ech = getch();
            if (ech == 10 || ech == KEY_ENTER) {
                input = 0;
            } else if (ech == KEY_LEFT) {
                cpos -= 1;
                if (cpos < 0)
                    cpos = 7;
            } else if (ech == KEY_RIGHT) {
                cpos += 1;
                if (cpos > 7)
                    cpos = 0;
            } else if (ech == ' ') {
                quasar.address ^= (1 << (7 - cpos));
            }
            attron(COLOR_PAIR(2) | A_STANDOUT);
            char ustr[23];
            printnum(quasar.address, ustr);
            mvprintw(quasar.buttony[0] - 13, 3, ustr);
            attroff(COLOR_PAIR(2) | A_STANDOUT);
        }
        attroff(A_STANDOUT);
        attron(COLOR_PAIR(2));
        char ustr[23];
        printnum(quasar.address, ustr);
        mvprintw(quasar.buttony[0] - 13, 3, ustr);
        attron(COLOR_PAIR(2));
        
        input = 1;
        cpos  = 0;
        /* Allow user to input data value */
        while (input) {
            if (cpos < 4)
                move(quasar.buttony[0] - 10, 3 + cpos);
            else 
                move(quasar.buttony[0] - 10, 4 + cpos);
            
            int ech = getch();
            if (ech == 10 || ech == KEY_ENTER) {
                input = 0;
            } else if (ech == KEY_LEFT) {
                cpos -= 1;
                if (cpos < 0)
                    cpos = 7;
            } else if (ech == KEY_RIGHT) {
                cpos += 1;
                if (cpos > 7)
                    cpos = 0;
            } else if (ech == ' ') {
                quasar.value ^= (1 << (7 - cpos));
            }
            attron(COLOR_PAIR(2) | A_STANDOUT);
            char ustr[23];
            printnum(quasar.value, ustr);
            mvprintw(quasar.buttony[0] - 10, 3, ustr);
            attroff(COLOR_PAIR(2) | A_STANDOUT);
        }
        quasar.mem[quasar.address] = quasar.value;
        quasar.deposit = 0x0;
    }
    
    /* Place cursor */
    move(quasar.buttony[quasar.cbutton], quasar.buttonx[quasar.cbutton]);
    refresh();

    /* Turn off single use buttons */
    if (quasar.single == 0x1) {
        quasar.single = 0x0;
        quasar.stop   = 0x1;
        sleep(1);
    } else if (quasar.rate == 0x1) {
        quasar.rateval *= 2;
        if (quasar.rateval > 8)
            quasar.rateval = 1;
        quasar.rate = 0x0;
        sleep(1);
    }
    
    /* Process user input */
    int ch = getch();
    if (ch == 'q' || ch == 'Q') {
        quasar.quit = 0x1;
    } else if (ch == 'i' || ch == 'I') {
        quasar.single = 0x1;
        quasar.stop   = 0x0;
        quasar.run    = 0x0;
    } else if (ch == 'r' || ch == 'R') {
        quasar.run    = 0x1;
        quasar.stop   = 0x0;
        quasar.single = 0x0;
    } else if (ch == 's' || ch == 'S') {
        quasar.run    = 0x0;
        quasar.stop   = 0x1;
        quasar.single = 0x0;
    } else if (ch == 'e' || ch == 'E') {
        quasar.examine = 0x1;
    } else if (ch == 'd' || ch == 'D') {
        quasar.deposit = 0x1;
    } else if (ch == 'a' || ch == 'A') {
        quasar.rate = 0x1;
    } else if (ch == KEY_LEFT || ch == KEY_DOWN) {
        quasar.cbutton -= 1;
        if (quasar.cbutton < 0){
            quasar.cbutton = 6;
        }
    } else if (ch == KEY_RIGHT || ch == KEY_UP) {
        quasar.cbutton += 1;
        if (quasar.cbutton > 6) {
            quasar.cbutton = 0;
        }
    } else if (ch == KEY_ENTER || ch == 10) {
        if (quasar.cbutton == 0) {
            printf("\a");
            quasar.stop   = 0x1;
            quasar.single = 0x0;
            quasar.run    = 0x0;
        } else if (quasar.cbutton == 1) {
            printf("\a");
            quasar.stop   = 0x0;
            quasar.single = 0x0;
            quasar.run    = 0x1;
        } else if (quasar.cbutton == 2) {
            printf("\a");
            quasar.stop   = 0x0;
            quasar.single = 0x1;
            quasar.run    = 0x0;
        } else if (quasar.cbutton == 3) {
            printf("\a");
            quasar.examine = 0x1;
        }  else if (quasar.cbutton == 4) {
            printf("\a");
            quasar.deposit = 0x1;
        }  else if (quasar.cbutton == 5) {
            printf("\a");
            quasar.rate = 0x1;
        }  else if (quasar.cbutton == 6) {
            printf("\a");
            quasar.quit = 0x1;
        }
    }
}

void init_display() {
    /* Initialize ncurses display */
	initscr();
    raw();
    cbreak();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    noecho();

    /* Colours */
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_WHITE);
    init_pair(2, COLOR_RED,   COLOR_WHITE);
    init_pair(3, COLOR_BLUE,  COLOR_WHITE);
    init_pair(4, COLOR_WHITE, COLOR_BLUE);

    /* Cursor position */
    quasar.cbutton = 0;

    /* Button positions */
    int xl, yl, dx;
    xl = 33 + NCOLS/10;
    yl = NROWS - 5;
    dx = NCOLS/10;
    quasar.buttonx[0] = xl;
    quasar.buttonx[1] = xl + 3 + dx;
    quasar.buttonx[2] = xl + 6 + 2*dx;
    quasar.buttonx[3] = xl;
    quasar.buttonx[4] = xl + 3 + dx;
    quasar.buttonx[5] = xl;
    quasar.buttonx[6] = xl + 3 + dx;
    quasar.buttony[0] = yl - 2;
    quasar.buttony[1] = yl - 2;
    quasar.buttony[2] = yl - 2;
    quasar.buttony[3] = yl;
    quasar.buttony[4] = yl;
    quasar.buttony[5] = yl + 2;
    quasar.buttony[6] = yl + 2;
}

void end_display() {
    /* Close ncurses windows */
    endwin();
}

void test_print() {
    /* Test print helper function (for debugging) */
    printf("\nCurrent system state:\n");
    printf("IR = 0x%X, IAR = 0x%X\n", quasar.IR, quasar.IAR);
    printf("R0 = 0x%X, R1 = 0x%X, R2 = 0x%X, R3 = 0x%x\n\n", quasar.R[0], 
           quasar.R[1], quasar.R[2], quasar.R[3]);
}
