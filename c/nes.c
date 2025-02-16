#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

// https://www.nesdev.org/wiki/Status_flags
#define SET_CARRY(p, value) ((p) = ((p) & ~0b00000001) | ((value) & 0b00000001))
#define SET_ZERO(p, value) ((p) = ((p) & ~0b00000010) | ((value) & 0b00000010))
#define SET_INTERRUPT(p, value) ((p) = ((p) & ~0b00000100) | ((value) & 0b00000100))
#define SET_DECIMAL(p, value) ((p) = ((p) & ~0b00001000) | ((value) & 0b00001000))
#define SET_BREAK(p, value) ((p) = ((p) & ~0b00010000) | ((value) & 0b00010000))
#define SET_OVERFLOW(p, value) ((p) = ((p) & ~0b01000000) | ((value) & 0b01000000))
#define SET_NEGATIVE(p, value) ((p) = ((p) & ~0b10000000) | ((value) & 0b10000000))

#define GET_CARRY(p) ((p) & 0b00000001)
#define GET_ZERO(p) ((p) & 0b00000010)
#define GET_INTERRUPT(p) ((p) & 0b00000100)
#define GET_DECIMAL(p) ((p) & 0b00001000)
#define GET_BREAK(p) ((p) & 0b00010000)
#define GET_OVERFLOW(p) ((p) & 0b01000000)
#define GET_NEGATIVE(p) ((p) & 0b10000000)

#define PC (cpu->registers.pc)

enum AddressingMode {
    IMMEDIATE,
    ABSOLUTE,
    ABSOLUTE_X,
    ABSOLUTE_Y,
    ZERO_PAGE,
    ZERO_PAGE_X,
    ZERO_PAGE_Y,
    INDIRECT_X,
    INDIRECT_Y
};

/* Stack */
#define pushStack(cpu, value) \
    _Generic((value), \
        uint8_t: pushStack_u8, \
        uint16_t: pushStack_u16 \
    )(cpu, value)

static inline void pushStack_u8(CPU* cpu, uint8_t value) {
    cpu->memory[0x100 + cpu->registers.s] = value;
    cpu->registers.s--;
}

static inline void pushStack_u16(CPU* cpu, uint16_t value) {
    pushStack_u8(cpu, (value >> 8) & 0xFF);
    pushStack_u8(cpu, value & 0xFF);
}

uint8_t popStack(CPU* cpu) {
    cpu->registers.s++;
    return cpu->memory[0x100 + cpu->registers.s];
}

typedef void (*instrFunc)();

// https://www.nesdev.org/wiki/CPU_addressing_modes
uint8_t readByte(uint16_t address);
uint16_t readWord(uint16_t address);
void writeByte(uint16_t address, uint8_t value);
void writeWord(uint16_t address, uint16_t value);
uint16_t getImmediate(uint16_t address);
uint16_t getAbsolute(uint16_t address);
uint16_t getAbsoluteX(uint16_t address);
uint16_t getAbsoluteY(uint16_t address);
uint16_t getZeroPage(uint16_t address);
uint16_t getZeroPageX(uint16_t address);
uint16_t getZeroPageY(uint16_t address);
uint16_t getIndirectX(uint16_t address);
uint16_t getIndirectY(uint16_t address);

// Instructions: https://www.masswerk.at/6502/6502_instruction_set.html#SLO
void ADC();

void ADD();

void AND(CPU* cpu, uint8_t value, uint8_t cycles) {
    cpu->registers.acc &= value;
    SET_ZERO(cpu->registers.p, cpu->registers.acc);
    SET_NEGATIVE(cpu->registers.p, cpu->registers.acc);
    cpu->clock->skipCycles += cycles;
}

void ASL();

void BCC();

void BCS();

void BEQ();

void BIT();

void BMI();

void BNE();

/* From: http://www.6502.org/users/obelisk/6502/reference.html
    * 
    * BPL - Branch if positive
    * If the negative flag is clear then add the relative displacement to the program counter to cause a branch to a new location.
    *
*/
void BPL() {

}

/* From: http://www.6502.org/users/obelisk/6502/reference.html
    * 
    * BRK - Break
    * Description: The BRK instruction forces the generation of an interrupt request. 
    * The program counter and processor status are pushed on the stack then the IRQ interrupt vector at $FFFE/F is loaded into the PC 
    * and the break flag in the status set to one.
    *
*/
void BRK(CPU* cpu) {
    pushStack(cpu, cpu->registers.pc);
    pushStack(cpu, cpu->registers.p);
    cpu->registers.pc = readWord(0xFFFE);
    SET_BREAK(cpu->registers.p, 1);
    cpu->clock->skipCycles += 7;
}

void BVC();

void BVS();

void CLC();

void CLD();

void CLI();

void CLV();

void CMP();

void CPX();

void CPY();

void DEC();

void DEX();

void DEY();

void EOR();

void INC();

void INX();

void INY();

void JMP();

void JSR();

void LDA();

void LDX();

void LDY();

void LSR();

void NOP();

/* From: http://www.6502.org/users/obelisk/6502/reference.html
    * 
    * ORA - Logical inclusive OR
    * An inclusive OR is performed, bit by bit, on the accumulator contents using the contents of a byte of memory.
    * A, Z, N = A | M
    *
*/
void ORA(CPU* cpu, uint8_t value, uint8_t cycles) {
    cpu->registers.acc |= value;
    SET_ZERO(cpu->registers.p, cpu->registers.acc);
    SET_NEGATIVE(cpu->registers.p, cpu->registers.acc);
    cpu->clock->skipCycles += cycles;
}

void PHA();

void PHP();

void PLA();

void PLP();

void ROL();

void ROR();

void RTI();

void RTS();

void SBC();

void SEC();

void SED();

void SEI();

void STA();

void STX();

void STY();

void TAX();

void TAY();

void TSX();

void TXA();

void TXS();

void TYA();

typedef struct clock {
    uint64_t cycles;
    uint64_t skipCycles;
} Clock;

typedef struct game_info {

} GameInformation;

typedef struct registers {
    uint8_t acc;
    uint8_t x;
    uint8_t y;
    uint16_t pc;
    uint8_t s;
    uint8_t p;
} regs;

typedef struct cpu {
    regs registers;
    uint8_t memory[0xFFFF];
    Graphics* graphics;
    Clock* clock;
} CPU;

typedef struct graphics {
    uint8_t screen[0xF000];
} Graphics;

/* 2D instruction table (only for reference) */
instrFunc instructions[256] = {
    BRK, ORA, NOP, NOP, NOP, ORA, ASL, NOP, PHP, ORA, ASL, NOP, NOP, ORA, ASL, NOP,
    BPL, ORA, NOP, NOP, NOP, ORA, ASL, NOP, CLC, ORA, NOP, NOP, NOP, ORA, ASL, NOP,
    JSR, AND, NOP, NOP, BIT, AND, ROL, NOP, PLP, AND, ROL, NOP, BIT, AND, ROL, NOP,
    BMI, AND, NOP, NOP, NOP, AND, ROL, NOP, SEC, AND, NOP, NOP, NOP, AND, ROL, NOP,
    RTI, EOR, NOP, NOP, NOP, EOR, LSR, NOP, PHA, EOR, LSR, NOP, JMP, EOR, LSR, NOP,
    BVC, EOR, NOP, NOP, NOP, EOR, LSR, NOP, CLI, EOR, NOP, NOP, NOP, EOR, LSR, NOP,
    RTS, ADC, NOP, NOP, NOP, ADC, ROR, NOP, PLA, ADC, ROR, NOP, JMP, ADC, ROR, NOP,
    BVS, ADC, NOP, NOP, NOP, ADC, ROR, NOP, SEI, ADC, NOP, NOP, NOP, ADC, ROR, NOP,
    NOP, STA, NOP, NOP, STY, STA, STX, NOP, DEY, NOP, TXA, NOP, STY, STA, STX, NOP,
    BCC, STA, NOP, NOP, STY, STA, STX, NOP, TYA, STA, TXS, NOP, NOP, STA, NOP, NOP,
    LDY, LDA, LDX, NOP, LDY, LDA, LDX, NOP, TAY, LDA, TAX, NOP, LDY, LDA, LDX, NOP,
    BCS, LDA, NOP, NOP, LDY, LDA, LDX, NOP, CLV, LDA, TSX, NOP, LDY, LDA, LDX, NOP,
    CPY, CMP, NOP, NOP, CPY, CMP, DEC
};

void executeInstruction(uint8_t opcode, CPU* cpu) {
    switch (opcode) {
        case 0x00: { BRK(cpu); break; }

        // ADC

        // AND
        case 0x29: { PC++; AND(cpu, getImmediate(PC), 2); PC++; break; }
        case 0x25: { PC++; AND(cpu, getZeroPage(PC), 3); PC++; break; }
        case 0x35: { PC++; AND(cpu, getZeroPageX(PC), 4); PC++; break; }
        case 0x2D: { PC++; AND(cpu, getAbsolute(PC), 4); PC += 2; break; }
        case 0x3D: { PC++; AND(cpu, getAbsoluteX(PC), 4); PC += 2; break; }
        case 0x39: { PC++; AND(cpu, getAbsoluteY(PC), 4); PC += 2; break; }
        case 0x21: { PC++; AND(cpu, getIndirectX(PC), 6); PC++; break; }
        case 0x31: { PC++; AND(cpu, getIndirectY(PC), 5); PC++; break; }

        // ASL
        case 0x0A: { PC++; ASL(cpu, 2); break; }
        case 0x06: { PC++; ASL(cpu, getZeroPage(PC), 5); PC++; break; }
        case 0x16: { PC++; ASL(cpu, getZeroPageX(PC), 6); PC++; break; }
        case 0x0E: { PC++; ASL(cpu, getAbsolute(PC), 6); PC += 2; break; }
        case 0x1E: { PC++; ASL(cpu, getAbsoluteX(PC), 7); PC += 2; break; }

        // ORA
        case 0x09: { PC++; ORA(cpu, getImmediate(PC), 2); PC++; break; }
        case 0x05: { PC++; ORA(cpu, getZeroPage(PC), 3); PC++; break; }
        case 0x15: { PC++; ORA(cpu, getZeroPageX(PC), 4); PC++; break; }
        case 0x0D: { PC++; ORA(cpu, getAbsolute(PC), 4); PC += 2; break; }
        case 0x1D: { PC++; ORA(cpu, getAbsoluteX(PC), 4); PC += 2; break; }
        case 0x19: { PC++; ORA(cpu, getAbsoluteY(PC), 4); PC += 2; break; }
        case 0x01: { PC++; ORA(cpu, getIndirectX(PC), 6); PC++; break; }
        case 0x11: { PC++; ORA(cpu, getIndirectY(PC), 5); PC++; break; }
    }
}

int main(int argc, char* argv[argc + 1]) {
    if (argc != 2) {
        fprintf(stderr, "Incorrect amount of cmd line arguments. Must only be ROM location\n");
    }
    
    GameInformation* gameInformation = (GameInformation*) malloc(sizeof(GameInformation));

    FILE* fileptr = fopen(argv[1], "rb");
    fseek(fileptr, 0, SEEK_END);
    long filelen = ftell(fileptr);
    rewind(fileptr);

    /* Load the ROM data */
    uint8_t* romData = (uint8_t*) malloc(filelen * sizeof(uint8_t));
    fread(romData, filelen, 1, fileptr);
    fclose(fileptr);

    CPU* cpu = (CPU*) malloc(sizeof(CPU));

    while (1) {
        uint8_t opcode = readByte(cpu->registers.pc);
        executeInstruction(opcode, cpu);
    }

    return EXIT_SUCCESS;
}