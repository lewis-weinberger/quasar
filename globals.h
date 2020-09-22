#ifndef _GLOBALS_
#define _GLOBALS_

struct System {
  /* Memory */
  unsigned char mem[256];

  /* Registers */
  unsigned char R[4], IR, IAR, FLAG;

  /* Console */
  unsigned char address, value;
  unsigned char stop, run, single;
  unsigned char examine, deposit; 

  /* User interface */
  unsigned char quit;
  unsigned char rate;
  int rateval;
  int cbutton;
  int buttonx[7], buttony[7];
  char log[4][15];
};
extern struct System quasar;

/* OPCODES */
#define LOAD  0x0 /* binary 0000 */
#define STORE 0x1 /* binary 0001 */
#define DATA  0x2 /* binary 0010 */
#define JMPR  0x3 /* binary 0011 */
#define JMP   0x4 /* binary 0100 */
#define JCAEZ 0x5 /* binary 0101 */
#define CLF   0x6 /* binary 0110 */
#define ADD   0x8 /* binary 1000 */ 
#define SHR   0x9 /* binary 1001 */
#define SHL   0xA /* binary 1010 */
#define NOT   0xB /* binary 1011 */
#define AND   0xC /* binary 1100 */
#define OR    0xD /* binary 1101 */
#define XOR   0xE /* binary 1110 */
#define CMP   0xF /* binary 1111 */

#endif // _GLOBALS_
