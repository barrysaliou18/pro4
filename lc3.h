/*
 * Authors: Brian Jorgenson
 *          Mamadou Barry
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
#define PSR_N_MASK 0x0004
#define PSR_Z_MASK 0x0002
#define PSR_P_MASK 0x0001
#define CLEAR_CC 0xFFF8
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
#define PSR_N_SHIFT 2
#define PSR_Z_SHIFT 1

/*----------------------Config-----*/
#define NO_OF_REGISTERS 8
#define MAXBREAKPOINTS 10
#define MAXMEM 0xFFFF
#define MINMEM 0x0000
#define MEM_DISPLAY 16
#define DEFAULT_PC 0x3000

/*---------Display Coordinates-----*/
#define MEMORY_X 30
#define MEMORY_Y 4
#define REGISTER_X 5
#define REGISTER_Y 4
#define USER_SELECTION_OPTIONS_Y 21
#define USER_INPUT_Y 22
#define DIVIDER_Y 23
#define INPUT_Y 24
#define OUTPUT_Y 25
#define S_REGISTER_Y 14
#define S_REGISTER_X 3
#define S_REGISTER_SPACING 12
#define OUTPUT_COLUMN 13
#define OUTPUT_ROW 25

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
    Register ir, pc, mar, mdr, psr;
    boolean ben;
    ALU_s alu;
} CPU_s;    // the _s designates the type as a structure

typedef CPU_s *CPU_p;

/*------------------Prototypes-----*/
void error(char *s);
char *checkDebugPointer(int i, CPU_p cpu);
int controller (CPU_p cpu);
int isBreakPoint(int address);
void parseIR(CPU_p cpu, Register opcode, Register* rd,  Register* rs1, Register* rs2);
void printScreen(CPU_p cpu);
int readInFile(char *fileName);
void setBEN(CPU_p cpu);
void setBreakPoint(int address);
void setCC(short compVal, CPU_p cpu);
void setImmMode(Register ir, Register *immMode);
void sext(CPU_p cpu, Register opcode, short *sext);
void userSelection(CPU_p cpu);
#endif /* LC3_H_ */