#ifndef LC3_H_
#define LC3_H_
#include <stdio.h>
#include <stdlib.h>
#define NO_OF_REGISTERS 8
#define MAXMEM 0xFFFF
#define MINMEM 0x0000
#define SIGN_EXT5 0xFFE0
#define SIGN_EXT6 0xFFC0
#define SIGN_EXT9 0xFE00
#define SIGN_EXT11 0xF800
#define N_MASK 0x0800
#define Z_MASK 0x0400
#define P_MASK 0x0200
#define BIT_FIVE_MASK 0x0020
#define BIT11_MASK 0x0800
#define IMMED5_MASK 0x001F
#define IMMED6_MASK 0x003F
#define IMMED9_MASK 0x01FF
#define IMMED11_MASK 0x07FF
#define ZEXT 0x00FF
#define DR_MASK 0x0E00
#define SR1_MASK 0x01C0
#define SR2_MASK 0x0007
#define FETCH 0
#define DECODE 1
#define EVAL_ADDR 2
#define FETCH_OP 3
#define EXECUTE 4
#define STORE 5
#define ADD 1
#define AND 5
#define NOT 9
#define TRAP 15
#define HALT 37
#define OUT 33
#define PUTS 34
#define GETC 32
#define LD 2
#define ST 3
#define JMP 12
#define BR 0
#define JSR 4
#define JSRR 4
#define LEA 14
#define STR 7
#define LOAD 1
#define RUN 2
#define STEP 3
#define DISMEM 5
#define BREAK 7
#define EXIT 9

typedef unsigned short Register;

typedef struct alu_s {
     Register a;
     Register b;
     Register r;
} ALU_s;

typedef struct cpu_s {
		Register reg_file[NO_OF_REGISTERS];
    Register ir;
    Register sext;
    unsigned short pc;
} CPU_s;    // the _s designates the type as a structure

typedef CPU_s *CPU_p;

static unsigned short MAR = 0;
static unsigned short MDR = 0;
static unsigned short rd = 0;
static unsigned short rs1 = 0;
static unsigned short rs2 = 0;
static unsigned short opcode;
static unsigned short immMode;
static unsigned short orig;
static unsigned short memPointer = 0x3000;
static unsigned int breakPoints[10];
static unsigned int memory[65535];
static unsigned int z = 0;
static unsigned int p = 0;
static unsigned int n = 0;
static ALU_s alu;
int programLoaded = 0;
int BEN = 0;
char* textOut;
int runEnabled = 0;

void parseIR(CPU_p cpu);
void sext(CPU_p cpu);
int controller (CPU_p cpu);
void setBEN(CPU_p cpu);
void setImmMode(Register ir);
void setCC(short compVal);
int readInFile(char *fileName);
void printScreen(CPU_p cpu);
void error(char *s);
void userSelection(CPU_p cpu);
char* checkDebugPointer(int i, CPU_p cpu);
char* append(char* str1, char* str2);
void setBreakPoint(int address);
int isBreakPoint(int address);
#endif /* LC3_H_ */