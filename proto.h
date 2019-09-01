#ifndef _PROTO_
#define _PROTO_

void init_emulator(int test, unsigned char *loadmem);
void init_display(void);
void parse(char *argv, unsigned char *loadmem);
void emulate(void);
void display(void);
void end_display(void);
void test_print(void);

#endif // _PROTO_
