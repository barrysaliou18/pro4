#include "lc3.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>

int main(int argc, char *argv[]) {
    CPU_p cpu = malloc(sizeof(CPU_s));
    cpu->pc = 0x3000; //initialize PC to 0
    cpu->ir = 0;
    setCC(0); //initialize cc nzp = 010
    int i;
    for(i = 0; i < 8; i++) {
        cpu->reg_file[i] = 0;
    }
    cpu->reg_file[1] = 0x0005;
    cpu->reg_file[2] = 0x000F;
    cpu->reg_file[3] = 0x4321;
    cpu->reg_file[4] = 0x1234;
    cpu->reg_file[5] = 0x1010;
    cpu->reg_file[6] = 0xf1f1;
    cpu->reg_file[7] = 0x7777;
    
    initscr();
    cbreak();
    for(;;) {
        printScreen(cpu);
        userSelection(cpu);
    }
    return 0;
}

//prints errors and waits for enter key
void error(char *s) {
    move(21, 2);
    clrtoeol();
    printw("%s", s);
    char c = 0;
    while(c != '\r' && c != '\n') {
        c = getch();
    }
}
//Reads in a file of hex coded lc3 instructions and places them into memeory
int readInFile(char *fileName) {
    FILE *file;
    file = fopen(fileName, "r");
    if (file == NULL) {
        return 1;
    }
    char buffer[5];
    int i = 0;
    while (fscanf(file, "%s", buffer) != EOF && i <= MAXMEM) {
        if (i == 0) {
            memPointer = strtol(buffer, NULL, 16);
            orig = memPointer;
            i = memPointer;
        } else {
            memory[i] = strtol(buffer, NULL, 16);
            i++;
        }
    }
    return 0;
}

int indexBP = 0;
void setBreakPoint(int address) {
    if (indexBP < 10) {
        breakPoints[indexBP++] = address;
    } else {
        error("Error: Too many break points. Press <ENTER> to continue.");
    }
}

int isBreakPoint(int address) {
    int i;
    for (i = 0; i < indexBP; i++) {
        if (breakPoints[i] == address) {
            return 1;
        }
    }
    return 0;
}

/*
 Prints all the information to the screen.
 */
void printScreen(CPU_p cpu) {
    int i;
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_RED);
    mvprintw(0, 5, "Welcome to the LC3 Simulator Simulator");
    mvprintw(2, 5, "Registers");
    for (i = 0; i < 8; i++) { //print out registers
        mvprintw((4 + i), 5, "R%d: x%04X", i, cpu->reg_file[i]);
    }
    
    mvprintw(2, 30, "Memory");
    int j;
    for (j = 0; j < 15; j++) { //print out memory
        mvprintw((4+j), 30, "%04X: x%04X", memPointer+j, memory[memPointer+j]);
        mvprintw((4+j), 27, checkDebugPointer(j, cpu));
        if (isBreakPoint(memPointer+j)) { //break points
            attron(COLOR_PAIR(1));
            mvprintw((4+j), 26, " ");
            attroff(COLOR_PAIR(1));
        } else {
            mvprintw((4+j), 26, " ");
        }
    }
    
    // specialty regesters
	mvprintw(14, 3, "PC:x%04X", cpu->pc);
	mvprintw(14, 15, "IR:x%04X", cpu->ir);
	mvprintw(15, 3, "MDR:x%04X", MDR);
	mvprintw(15, 15, "MAR:x%04X", MAR);
	mvprintw(16, 3, "A:x%04X", alu.a);
	mvprintw(16, 15, "B:x%04X", alu.b);
	mvprintw(17, 3, "CC: N: %d Z: %d P: %d", n, z, p);
    
    //user options
    mvprintw(20, 1, "Select: 1)Load, 2)Run, 3)Step, 5)Display Mem, 7)Break Points, 9)Exit");
    
    //selection input
    move(21,1);
    clrtoeol();
    mvprintw(21, 1, ">");
    
    //divider
    mvprintw(22, 1, "----------------------------------------------------");
    
    //program input/output area
    mvprintw(23, 5, "Input:");
    mvprintw(24, 5, "Output:");
    
    //reset cursor position
    move(21, 2);
}

char* checkDebugPointer(int i, CPU_p cpu) {
    return (memPointer+i) == cpu->pc ? "->x" : "  x";
}

void userSelection(CPU_p cpu) {
    char fileName[256];
    int status;
    int input;
    char str[5];
    char selection[2];
    getstr(selection);
    int select;
    sscanf(selection, "%d", &select);
    switch(select) {
        case LOAD:
            printScreen(cpu);
            printw("1 File name: ");
            getstr(fileName);
            status = readInFile(fileName);
            if(status) {
                printScreen(cpu);
                error("Error: File not found. Press <ENTER> to continue.");
            } else {
                programLoaded = 1; //enable program stepping
                cpu->pc = memPointer;
                setCC(0); //set cc nzp = 010
            }
            break;
        case RUN:
            printScreen(cpu);
            if (programLoaded) {
                runEnabled = 1;
                while (runEnabled) {
                    controller(cpu);
                } 
            } else {
                error("Error: Program not loaded. Press <ENTER> to continue.");
            }
            break;
        case STEP:
            printScreen(cpu);
            if(programLoaded) {
                controller(cpu);
            } else {
                error("Error: Program not loaded. Press <ENTER> to continue.");
            }
            break;
        case DISMEM:
            printScreen(cpu);
            printw("5 Go To Address: 0x");
            getstr(str);
            sscanf(str, "%x", &input);
            //scanf("%x", &input);
            if (input < MINMEM || input > 0xffef) {
                error("Error: Out of memory range. Press <ENTER> to continue.");
            } else {
                memPointer = input;
            }
            break;
        case BREAK:
            printScreen(cpu);
            printw("7 Enter Address For Break Point: 0x");
            getstr(str);
            sscanf(str, "%x", &input);
            if (input < MINMEM || input > MAXMEM) {
                error("Error: Out of memory range. Press <ENTER> to continue.");
            } else {
                setBreakPoint(input);
            }
            break;
        case EXIT:
            printScreen(cpu);
            printw("9 Thanks for using LC3 Simulator Simulator!");
            getch();
            free(cpu);
            endwin();
            exit(0);
            break;
        default:
            break;
    }
}

void parseIR(CPU_p cpu) {
    if(opcode == ADD || opcode == AND || opcode == NOT) {
        rd = (cpu->ir & DR_MASK) >> 9;
        rs1 = (cpu->ir & SR1_MASK) >> 6;
        rs2 = cpu->ir & SR2_MASK;
    }
    else if (opcode == STR) {
        rd = (cpu->ir & DR_MASK) >> 9;
        rs1 = (cpu->ir & SR1_MASK) >> 6;
    }
    else if(opcode == LD || opcode == LEA) {
        rd = (cpu->ir & DR_MASK) >> 9;
    }
    else if(opcode == ST) {
        rs1 = (cpu->ir & DR_MASK) >> 9;
    }
    else if(opcode == JMP || opcode == JSR) {
        rs1 = (cpu->ir & SR1_MASK) >> 6;
    }
}

/* sext
 *  Takes the 7 low-bits from the IR and bus the result to the
 *  sign extension Register and masks the 7 low-bits if there is a
 *  1 in the highest bit. Returns the sign extension Register.
 */
void sext(CPU_p cpu) {
    Register immed = cpu->ir;
    if(opcode == ADD || opcode == AND){
        immed &= IMMED5_MASK;
        if((immed >> 4) == 1) {
            immed |= SIGN_EXT5;
        }
    }
    else if (opcode == STR) {
        immed &= IMMED6_MASK;
        if ((immed >> 5) == 1) {
            immed |= SIGN_EXT6;
        }
    }
    else if(opcode == LD || opcode == ST || opcode == BR || opcode == LEA) {
        immed &= IMMED9_MASK;
        if((immed >> 8) == 1) {
            immed |= SIGN_EXT9;
        }
    } else if (opcode == JSR) {
        immed &= IMMED11_MASK;
        if ((immed >> 10) == 1) {
            immed |= SIGN_EXT11;
        }
    }
    cpu->sext = immed;
}
void setBEN(CPU_p cpu) {
    if(((cpu->ir & N_MASK) && n)
       || ((cpu->ir & Z_MASK) && z)
       || ((cpu->ir & P_MASK) && p)) {
        BEN = 1;
        //		printf("branch enabled\n");
    }
    else {
        BEN = 0;
        //		printf("branch not enabled\n");
    }
}
void setImmMode(Register ir) {
    if (ir & BIT_FIVE_MASK){ //[5] = 1
        immMode = 1;
    } else {
        immMode = 0;
    }
}

void setCC(short compVal) {
    if (compVal < 0) {
        n = 1;
        z = 0;
        p = 0;
    } else if (compVal) {
        n = 0;
        z = 0;
        p = 1;
    } else {
        n = 0;
        z = 1;
        p = 0;
    }
}

int row = 0;
int column = 0;
void output(char c) {
    move(24 + column, 13 + row);
    printw("%c", c);
    if (c == '\n') {
        row = 0;
        column++;
    } else {
        row++;
    }
}

int controller (CPU_p cpu) {
    // do any initializations here
    int state = FETCH;
    for (;;) {   // efficient endless loop
        switch (state) {
            case FETCH: // microstates 18, 33, 35 in the book
                MAR = cpu->pc;
                cpu->pc += 1;
                MDR = memory[MAR];
                cpu->ir = MDR;
               	state = DECODE;
                break;
            case DECODE: // microstate 32
                setBEN(cpu);
                setImmMode(cpu->ir);
                opcode = cpu->ir >> 12;
                state = EVAL_ADDR;
                break;
            case EVAL_ADDR: // Look at the LD instruction to see microstate 2 example
                sext(cpu);
                switch (opcode) {
                        // different opcodes require different handling
                        // compute effective address, e.g. add sext(immed7) to register
                    case ADD: //nothing to do
                        break;
                    case AND: //nothing to do
                        break;
                    case NOT: //nothing to do
                        break;
                    case TRAP:
                        MAR = cpu->ir & ZEXT;
                        MDR = memory[MAR];
                        cpu->reg_file[7] = cpu->pc;
                        //cpu->pc = MDR;
                        break;
                    case LD:
                        MAR = cpu->pc + cpu->sext;
                        break;
                    case ST:
                        MAR = cpu->pc + cpu->sext;
                        break;
                    case JMP: //nothing to do
                        break;
                    case BR: //nothing to do
                        break;
                    case JSR:
                        cpu->reg_file[7] = cpu->pc;
                        break;
                    case LEA:
                        break;
                    case STR:
                        alu.a = cpu->reg_file[rs1];
                        alu.b = cpu->sext;
                        break;
                }
                state = FETCH_OP;
                break;
            case FETCH_OP: // Look at ST. Microstate 23 example of getting a value out of a register
                parseIR(cpu);
                switch (opcode) {
                        // get operands out of registers into A, B of ALU
                        // or get memory for load instr.
                    case ADD:
                        if (immMode) {
                            alu.a = cpu->reg_file[rs1];
                            alu.b = cpu->sext;
                        } else {
                            alu.a = cpu->reg_file[rs1];
                            alu.b = cpu->reg_file[rs2];
                        }
                        break;
                    case AND:
                        if (immMode) {
                            alu.a = cpu->reg_file[rs1];
                            alu.b = cpu->sext;
                        } else {
                            alu.a = cpu->reg_file[rs1];
                            alu.b = cpu->reg_file[rs2];
                        }
                        break;
                    case NOT:
                        alu.a = cpu->reg_file[rs1];
                        break;
                    case TRAP: //nothing to do
                        break;
                    case LD:
                        MDR = memory[MAR];
                        break;
                    case ST:
                        MDR = cpu->reg_file[rs1];
                        break;
                    case JMP: //nothing to do
                        break;
                    case BR: //nothing to do
                        break;
                    case JSR:
                        if(cpu->ir & BIT11_MASK) { //bit 11 == 1
                            alu.a = cpu->pc;
                            alu.b = cpu->sext;
                        } else {
                            alu.a = 0;
                            alu.b = cpu->reg_file[rs1];
                        }
                        break;
                    case LEA:
<<<<<<< HEAD
=======
                        alu.a = cpu->pc;
                        alu.b = cpu->sext;
>>>>>>> origin/master
                        break;
                }
                state = EXECUTE;
                break;
            case EXECUTE: // Note that ST does not have an execute microstate
                switch (opcode) {
                        // do what the opcode is for, e.g. ADD
                        // in case of TRAP: call trap(int trap_vector) routine, see below for TRAP x25 (HALT)
                    case ADD:
                        alu.r = alu.a + alu.b;
                        break;
                    case AND:
                        alu.r = alu.a & alu.b;
                        break;
                    case NOT:
                        alu.r = ~(alu.a);
                        break;
                    case TRAP: //nothing to do
                        break;
                    case LD: //nothing to do
                        break;
                    case ST: //nothing to do
                        break;
                    case JMP: //also RET
                        alu.r = rs1;
                        break;
                    case BR:
                        if(BEN){
                            alu.r = cpu->pc + cpu->sext;
                        }
                        break;
                    case JSR:
                        alu.r = alu.a + alu.b;
                        break;
                    case LEA:
                        alu.r = alu.a + alu.b;
                        break;
                    case STR:
                        alu.r = alu.a + alu.b;
                        break;
                }
                state = STORE;
                break;
            case STORE: // Look at ST. Microstate 16 is the store to memory
                switch (opcode) {
                        // write back to register or store MDR into memory
                    case ADD: //sets cc
                        cpu->reg_file[rd] = alu.r;
                        setCC(alu.r);
                        break;
                    case AND: //sets cc
                        cpu->reg_file[rd] = alu.r;
                        setCC(alu.r);
                        break;
                    case NOT: //sets cc
                        cpu->reg_file[rd] = alu.r;
                        setCC(alu.r);
                        break;
                    case TRAP:
                        switch (cpu->ir & ZEXT) {
                            case HALT: // 0x25
                                cpu->pc = orig;
                                runEnabled = 0; 
                                break;
                            case OUT: { // 0x21
                                char input = cpu->reg_file[0];
                                output(input);
                                break;
                            }
                            case GETC: // 0x20
                                printScreen(cpu);
                                printw("Enter a character.");
                                cpu->reg_file[0] = getch();
                                break;
                            case PUTS: { // 0x22
                                int i = 0;
                                Register r0 = cpu->reg_file[0];
                                char c = 1;
                                for (; c != '\0'; i++) {
                                    c = memory[r0 + i];
                                    output(c);
                                }
                                break;
                            }
                            default:
                                break;
                        }
                        break;
                    case LD: //sets cc
                        cpu->reg_file[rd] = MDR;
                        setCC(MDR);
                        break;
                    case ST:
                        memory[MAR] = MDR;
                        break;
                    case JMP:
                        cpu->pc = alu.r;
                        break;
                    case BR:
                        if(BEN){
                            cpu->pc = alu.r;
                        }
                        break;
                    case JSR:
                        cpu->pc = alu.r;
                        break;
                    case LEA:
                        cpu->reg_file[rd] = alu.r;
                        setCC(alu.r);
                        break;
                    case STR:
                        memory[alu.r] = cpu->reg_file[rd];
                        break;
                }
                // do any clean up here in prep for the next complete cycle
                state = FETCH;
                if(isBreakPoint(cpu->pc)) {
                    runEnabled = 0;
                }
                return 0;
        }
    }
}