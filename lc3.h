/*
 * Author: Brian Jorgenson
 *
 */

#ifndef LC3_H_
#define LC3_H_
#include <stdio.h>
#include <stdlib.h>

/*--------------------Op Codes-----*/
#define BR 0
#define ADD 1
#define LD 2
#define ST 3
#define JSR 4
#define JSRR 4
#define AND 5
#define LDR 6
#define STR 7
#define NOT 9
#define JMP 12
#define LEA 14
#define TRAP 15

/*----------------Trap Vectors-----*/
#define HALT 0x25
#define OUT 0x21
#define PUTS 0x22
#define GETC 0x20

/*--------------User Selection-----*/
#define LOAD 1
#define RUN 2
#define STEP 3
#define DISMEM 5
#define BREAK 7
#define EXIT 9

/*--------------Machine States-----*/
#define FETCH 0
#define DECODE 1
#define EVAL_ADDR 2
#define FETCH_OP 3
#define EXECUTE 4
#define STORE 5

/*-----------------------Masks-----*/
#define N_MASK 0x0800
#define Z_MASK 0x0400
#define P_MASK 0x0200
#define BIT_FIVE_MASK 0x0020
#define BIT11_MASK 0x0800
#define IMMED5_MASK 0x001F
#define IMMED6_MASK 0x003F
#define IMMED9_MASK 0x01FF
#define IMMED11_MASK 0x07FF
#define DR_MASK 0x0E00
#define SR1_MASK 0x01C0
#define SR2_MASK 0x0007

/*-------------Sign Extentions-----*/
#define SIGN_EXT5 0xFFE0
#define SIGN_EXT6 0xFFC0
#define SIGN_EXT9 0xFE00
#define SIGN_EXT11 0xF800
#define ZEXT 0x00FF

/*-------------Register Shifts-----*/
#define DR_SHIFT 9
#define SR1_SHIFT 6
#define OPCODE_SHIFT 12
#define IMMED5_SIGN_SHIFT 4
#define IMMED6_SIGN_SHIFT 5
#define IMMED9_SIGN_SHIFT 8
#define IMMED11_SIGN_SHIFT 10

/*----------------------Config-----*/
#define NO_OF_REGISTERS 8
#define MAXBREAKPOINTS 10
#define MAXMEM 0xFFFF
#define MINMEM 0x0000
#define MEM_DISPLAY 16

/*--------------------Typedefs-----*/
typedef uint8_t boolean;
#define true 1
#define false 0

typedef uint16_t Register;

typedef struct alu_s {
    Register a, b, r;
} ALU_s;

typedef struct cpu_s {
    Register reg_file[NO_OF_REGISTERS];
    Register ir, pc, MAR, MDR;
    boolean BEN;
    ALU_s alu;
} CPU_s;    // the _s designates the type as a structure

typedef CPU_s *CPU_p;

/*------------------Prototypes-----*/
void parseIR(CPU_p cpu, Register opcode);
void sext(CPU_p cpu, Register opcode, short* sext);
int controller (CPU_p cpu);
void setBEN(CPU_p cpu);
void setImmMode(Register ir, Register* immMode);
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