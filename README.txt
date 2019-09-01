QUASAR -- QUite A Small ARchitecture
####################################

ABOUT
#####
QUASAR is an emulator for a basic computer architecture based on the system
outlined in "But How Do It Know?" by J. Clark Scott. It is specifically
both a parser for assembly code written using the system's instruction set and
also an emulator of a simple setup with CPU and memory. 

The architecture has four (8-bit) general purpose registers labelled R0 to R3.
Other registers include an instruction register (IR), 
an instruction address register (IAR) and a flag register (FLAG). 

The memory data width is 8 bits and the address width is also 8 bits. 
This means the total memory is 2048 bits (or 256 8-bit bytes). The memory
layout is big-endian (most significant bit first).

Instructions are 8 bits wide and include the following:
    * ADD   RA, RB   (add value in register A to register B)
    * SHR   RA, RB   (shift value in RA right)
    * SHL   RA, RB   (shift value in RA left)
    * NOT   RA, RB   (logical NOT value in RA)
    * AND   RA, RB   (logical AND the values in RA and RB)
    * OR    RA, RB   (logical OR the values in RA and RB)
    * XOR   RA, RB   (logical XOR the values in RA and RB)
    * CMP   RA, RB   (compare RA and RB, with result in FLAG register)
    * LOAD  RA, RB   (load value to RB from memory address in RA)
    * STORE RA, RB   (store value from RB to memory at address in RA)
    * DATA  RB, Addr (load 8 bits from next memory address into RB)
    * JMPR  RB       (jump to instruction at address in RB) 
    * JMP   Addr     (jump to instruction at address)
    * JCAEZ Addr     (jump if, for flags CARRY, LARGER THAN, EQUAL TO, ZERO)
    * CLF            (clear flags)
Note that where an output is generated it is stored in the second register 
argument (RB). Also note that the DATA and JMP commands need to be 16 bits
wide (since the data/address width is 8 bits), and so these actually take two
instructions, with the value stored at the next address in the program. If the
ALU instructons (like ADD) result in a number larger than 255, this will be
turn on the CARRY bit in the FLAG register. Similarly for the CMP instruction
if the result is LARGER THAN, EQUAL TO then the FLAG register will store the
output. This is then used if one of the JCAEZ family of commands is employed.
The default machine opcodes for the instructions are set in globals.h.

INSTALLATION
############
QUASAR is designed for use on *NIX based systems. It is written in C and takes
advantage of the NCURSES library to generate a user interface. To install,
simply run make in the source directory; this should generate an executable
called "quasar". Futher configuration options are discussed below.

RUNNING
#######
QUASAR can be run in two modes:
    1. It can be run standalone, giving the executable no command line options.
    In this case the application will present a console to interact with the
    emulator. This allows the user to input data into memory directly, and then
    run whatever has been inputed. In this mode the memory is initially zeroed
    at all addresses.
    
    2. It can be run with a script, by giving the path to the script
    as a command line argument. This script can be written directly in binary
    machine code, or in QUASAR assembly code. The parser will then translate the 
    script into machine code and load the program into the emulator's memory at
    address 0, ready to be run.

    (3. There is a third option, using the command line switch --test. This will
    start the system as in option 1, but will also load a simple test program
    into memory at address 0. This can be used to play around with the system
    and to check it is running properly!)

SCRIPTS
#######
The parser is very simple, so the formatting of scripts is heavily constrained.
If writing in binary machine code, the parser will only read the first 8
characters of each line, expecting them to be 1s and 0s (it will simply ignore
other characters as if they are zeroes). To indicate to the parser that your
script is in machine code you must save the file with extension .qmc (e.g. 
mycode.qmc). Since only the first 8 characters of each line are loaded into
memory, the remaining columns of the script are effectively free to use as
comments.
Similarly if writing in assembly script, the extension .qasm must be used. Again
like for the machine code scripts, a fixed width is read for each line, but this
time it is 11 characters. Each line should consist of the operation in capitals,
followed by the register operands in capitals, and each term should be separated
by spaces. For example, the longest possible line is "STORE R0 R1" (hence the
maximum width of 11 characters). Anything beyond this is not read so can be
used for commenting. If the parser does not recognise part of your script it 
will print the line it has struggled with.

CONSOLE
#######
The emulator console contains a lot of information and must be used to interact
with the QUASAR system. The console presents the user with 2 data words shown
as their binary value which can be changed by the user:
    1. ADDRESS, a user-chosen memory address,
    2. DATA, the value stored at that address,
It also shows the current values of the 7 registers. Alongside these there are 
6 buttons for interacting with the system:
    1. QUIT, which will quit the system,
    2. RUN, which will start the system from the address in the IAR,
    3. STOP, which will pause the system,
    4. SINGLE-STEP, which will increment the system one cycle,
    5. EXAMINE, which will print the value at the chosen memory address,
    6. DEPOSIT, which will change the value at the chosen memory address.
The user can manually enter values into memory by choosing a memory address
and using DEPOSIT to alter the value stored at that address. Only the first
two of the data words displayed can be changed manually by the user from the 
console.
There is also a final input for the user to alter the sytem, which is the RATE
button. This controls the cycle rate of the system, which by default is set to
be sufficiently slow that the user can easily see as the system cycles. The rate
value can be increased to 2x, 4x, and 8x, if the user would like to speed up
the system.
Interaction with the console is achieved via the keyboard. The keypad buttons
(LEFT/RIGHT/UP/DOWN) and ENTER key allow the user to select different functions.
These functions can also be activated by pressing the key indicated by the 
square brackets (for example the q key for the [Q]UIT function). If the user
activates the EXAMINE/DEPOSIT functions then the ADDRESS/DATA values will be
highlighted, and the cursor can be moved to select different binary bits.
The value of a bit can then be toggled by pressing SPACE. When the desired value
is entered, the user should press ENTER to feed it to the system.

CONFIGURATION
#############
The system can be further configured at compile time by editing the values
in config.h. Possible options include:
    - DELAY_LOOP: parameter which controls how many execution loops to skip in
    between running the emulator. This allows the instruction cycle rate of the
    emulator to be slowed down to humanly-observable speed. The larger the
    number the slower the emulator will run.
    - NCOLS, NROWS: these define the size of the console UI in terms of 
    terminal characters and lines respectively.

TO-DO
#####
Features not yet implemented include:
    - Parser.
