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
uint8_t getImmediate(uint16_t address);
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

void ASL(CPU* cpu, uint8_t value, uint8_t cycles) {
    SET_CARRY(cpu->registers.p, cpu->registers.acc & 0x80);
    cpu->registers.acc <<= 1;
    SET_ZERO(cpu->registers.p, cpu->registers.acc);
    SET_NEGATIVE(cpu->registers.p, cpu->registers.acc);
    cpu->clock->skipCycles += cycles;
}

void BCC(CPU* cpu) {
    if (!GET_CARRY(cpu->registers.p)) {
        cpu->registers.pc += getImmediate(cpu->registers.pc);
    }
}

void BCS(CPU* cpu) {
    if (GET_CARRY(cpu->registers.p)) {
        cpu->registers.pc += getImmediate(cpu->registers.pc);
    }
}

void BEQ(CPU* cpu) {
    if (GET_ZERO(cpu->registers.p)) {
        cpu->registers.pc += getImmediate(cpu->registers.pc);
    }
}

void BIT(CPU* cpu, uint8_t value, uint8_t cycles) {
    SET_ZERO(cpu->registers.p, cpu->registers.acc & value);
    SET_OVERFLOW(cpu->registers.p, value & 0x40);
    SET_NEGATIVE(cpu->registers.p, value & 0x80);
    cpu->clock->skipCycles += cycles;
}

void BMI(CPU* cpu) {
    if (GET_NEGATIVE(cpu->registers.p)) {
        cpu->registers.pc += getImmediate(cpu->registers.pc);
    }
}

void BNE(CPU* cpu) {
    if (!GET_ZERO(cpu->registers.p)) {
        cpu->registers.pc += getImmediate(cpu->registers.pc);
    }
}

void BPL(CPU* cpu) {
    if (!GET_NEGATIVE(cpu->registers.p)) {
        cpu->registers.pc += getImmediate(cpu->registers.pc);
    }
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

void BVC(CPU* cpu) {
    if (!GET_OVERFLOW(cpu->registers.p)) {
        cpu->registers.pc += getImmediate(cpu->registers.pc);
    }
}

void BVS(CPU* cpu) {
    if (GET_OVERFLOW(cpu->registers.p)) {
        cpu->registers.pc += getImmediate(cpu->registers.pc);
    }
}

void CLC(CPU* cpu) {
    SET_CARRY(cpu->registers.p, 0);
}

void CLD(CPU* cpu) {
    SET_DECIMAL(cpu->registers.p, 0);
}

void CLI(CPU* cpu) {
    SET_INTERRUPT(cpu->registers.p, 0);
}

void CLV(CPU* cpu) {
    SET_OVERFLOW(cpu->registers.p, 0);
}

void CMP(CPU* cpu, uint8_t value, uint8_t cycles) {
    uint8_t result = cpu->registers.acc - value;
    SET_CARRY(cpu->registers.p, cpu->registers.acc >= value);
    SET_ZERO(cpu->registers.p, result == 0);
    SET_NEGATIVE(cpu->registers.p, result);
    cpu->clock->skipCycles += cycles;
}

void CPX(CPU* cpu, uint8_t value, uint8_t cycles) {
    uint8_t result = cpu->registers.x - value;
    SET_CARRY(cpu->registers.p, cpu->registers.x >= value);
    SET_ZERO(cpu->registers.p, result == 0);
    SET_NEGATIVE(cpu->registers.p, result);
    cpu->clock->skipCycles += cycles;
}

void CPY(CPU* cpu, uint8_t value, uint8_t cycles) {
    uint8_t result = cpu->registers.y - value;
    SET_CARRY(cpu->registers.p, cpu->registers.y >= value);
    SET_ZERO(cpu->registers.p, result == 0);
    SET_NEGATIVE(cpu->registers.p, result);
    cpu->clock->skipCycles += cycles;
}

void DEC(CPU* cpu, uint16_t address, uint8_t cycles) {
    uint8_t value = readByte(address) - 1;
    writeByte(address, value);
    SET_ZERO(cpu->registers.p, value == 0);
    SET_NEGATIVE(cpu->registers.p, value);
    cpu->clock->skipCycles += cycles;
}

void DEX(CPU* cpu) {
    cpu->registers.x--;
    SET_ZERO(cpu->registers.p, cpu->registers.x == 0);
    SET_NEGATIVE(cpu->registers.p, cpu->registers.x);
}

void DEY(CPU* cpu) {
    cpu->registers.y--;
    SET_ZERO(cpu->registers.p, cpu->registers.y == 0);
    SET_NEGATIVE(cpu->registers.p, cpu->registers.y);
}

void EOR(CPU* cpu, uint8_t value, uint8_t cycles) {
    cpu->registers.acc ^= value;
    SET_ZERO(cpu->registers.p, cpu->registers.acc == 0);
    SET_NEGATIVE(cpu->registers.p, cpu->registers.acc);
    cpu->clock->skipCycles += cycles;
}

void INC(CPU* cpu, uint16_t address, uint8_t cycles) {
    uint8_t value = readByte(address) + 1;
    writeByte(address, value);
    SET_ZERO(cpu->registers.p, value == 0);
    SET_NEGATIVE(cpu->registers.p, value);
    cpu->clock->skipCycles += cycles;
}

void INX(CPU* cpu) {
    cpu->registers.x++;
    SET_ZERO(cpu->registers.p, cpu->registers.x == 0);
    SET_NEGATIVE(cpu->registers.p, cpu->registers.x);
}

void INY(CPU* cpu) {
    cpu->registers.y++;
    SET_ZERO(cpu->registers.p, cpu->registers.y == 0);
    SET_NEGATIVE(cpu->registers.p, cpu->registers.y);
}

void JMP(CPU* cpu, uint16_t address) {
    cpu->registers.pc = address;
}

void JSR(CPU* cpu, uint16_t address) {
    pushStack(cpu, (uint16_t)(cpu->registers.pc - 1));
    cpu->registers.pc = address;
}

void LDA(CPU* cpu, uint8_t value, uint8_t cycles) {
    cpu->registers.acc = value;
    SET_ZERO(cpu->registers.p, cpu->registers.acc == 0);
    SET_NEGATIVE(cpu->registers.p, cpu->registers.acc);
    cpu->clock->skipCycles += cycles;
}

void LDX(CPU* cpu, uint8_t value, uint8_t cycles) {
    cpu->registers.x = value;
    SET_ZERO(cpu->registers.p, cpu->registers.x == 0);
    SET_NEGATIVE(cpu->registers.p, cpu->registers.x);
    cpu->clock->skipCycles += cycles;
}

void LDY(CPU* cpu, uint8_t value, uint8_t cycles) {
    cpu->registers.y = value;
    SET_ZERO(cpu->registers.p, cpu->registers.y == 0);
    SET_NEGATIVE(cpu->registers.p, cpu->registers.y);
    cpu->clock->skipCycles += cycles;
}

void LSR(CPU* cpu, uint8_t value, uint8_t cycles) {
    SET_CARRY(cpu->registers.p, value & 0x01);
    value >>= 1;
    SET_ZERO(cpu->registers.p, value == 0);
    SET_NEGATIVE(cpu->registers.p, value);
    cpu->clock->skipCycles += cycles;
}

void NOP(CPU* cpu) {
    // No operation
}

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

void PHA(CPU* cpu) {
    pushStack(cpu, cpu->registers.acc);
}

void PHP(CPU* cpu) {
    pushStack(cpu, cpu->registers.p);
}

void PLA(CPU* cpu) {
    cpu->registers.acc = popStack(cpu);
    SET_ZERO(cpu->registers.p, cpu->registers.acc == 0);
    SET_NEGATIVE(cpu->registers.p, cpu->registers.acc);
}

void PLP(CPU* cpu) {
    cpu->registers.p = popStack(cpu);
}

void ROL(CPU* cpu, uint8_t value, uint8_t cycles) {
    uint8_t carry = GET_CARRY(cpu->registers.p);
    SET_CARRY(cpu->registers.p, value & 0x80);
    value = (value << 1) | carry;
    SET_ZERO(cpu->registers.p, value == 0);
    SET_NEGATIVE(cpu->registers.p, value);
    cpu->clock->skipCycles += cycles;
}

void ROR(CPU* cpu, uint8_t value, uint8_t cycles) {
    uint8_t carry = GET_CARRY(cpu->registers.p);
    SET_CARRY(cpu->registers.p, value & 0x01);
    value = (value >> 1) | (carry << 7);
    SET_ZERO(cpu->registers.p, value == 0);
    SET_NEGATIVE(cpu->registers.p, value);
    cpu->clock->skipCycles += cycles;
}

void RTI(CPU* cpu) {
    cpu->registers.p = popStack(cpu);
    cpu->registers.pc = popStack(cpu);
}

void RTS(CPU* cpu) {
    cpu->registers.pc = popStack(cpu) + 1;
}

void SBC(CPU* cpu, uint8_t value, uint8_t cycles) {
    uint16_t result = cpu->registers.acc - value - (1 - GET_CARRY(cpu->registers.p));
    SET_CARRY(cpu->registers.p, result < 0x100);
    SET_ZERO(cpu->registers.p, (result & 0xFF) == 0);
    SET_OVERFLOW(cpu->registers.p, ((cpu->registers.acc ^ result) & (cpu->registers.acc ^ value) & 0x80) != 0);
    SET_NEGATIVE(cpu->registers.p, result);
    cpu->registers.acc = result & 0xFF;
    cpu->clock->skipCycles += cycles;
}

void SEC(CPU* cpu) {
    SET_CARRY(cpu->registers.p, 1);
}

void SED(CPU* cpu) {
    SET_DECIMAL(cpu->registers.p, 1);
}

void SEI(CPU* cpu) {
    SET_INTERRUPT(cpu->registers.p, 1);
}

void STA(CPU* cpu, uint16_t address, uint8_t cycles) {
    writeByte(address, cpu->registers.acc);
    cpu->clock->skipCycles += cycles;
}

void STX(CPU* cpu, uint16_t address, uint8_t cycles) {
    writeByte(address, cpu->registers.x);
    cpu->clock->skipCycles += cycles;
}

void STY(CPU* cpu, uint16_t address, uint8_t cycles) {
    writeByte(address, cpu->registers.y);
    cpu->clock->skipCycles += cycles;
}

void TAX(CPU* cpu) {
    cpu->registers.x = cpu->registers.acc;
    SET_ZERO(cpu->registers.p, cpu->registers.x == 0);
    SET_NEGATIVE(cpu->registers.p, cpu->registers.x);
}

void TAY(CPU* cpu) {
    cpu->registers.y = cpu->registers.acc;
    SET_ZERO(cpu->registers.p, cpu->registers.y == 0);
    SET_NEGATIVE(cpu->registers.p, cpu->registers.y);
}

void TSX(CPU* cpu) {
    cpu->registers.x = cpu->registers.s;
    SET_ZERO(cpu->registers.p, cpu->registers.x == 0);
    SET_NEGATIVE(cpu->registers.p, cpu->registers.x);
}

void TXA(CPU* cpu) {
    cpu->registers.acc = cpu->registers.x;
    SET_ZERO(cpu->registers.p, cpu->registers.acc == 0);
    SET_NEGATIVE(cpu->registers.p, cpu->registers.acc);
}

void TXS(CPU* cpu) {
    cpu->registers.s = cpu->registers.x;
}

void TYA(CPU* cpu) {
    cpu->registers.acc = cpu->registers.y;
    SET_ZERO(cpu->registers.p, cpu->registers.acc == 0);
    SET_NEGATIVE(cpu->registers.p, cpu->registers.acc);
}

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

void executeInstruction(uint8_t opcode, CPU* cpu) {
    switch (opcode) {
        case 0x00: { BRK(cpu); break; }

        // ADC
        case 0x69: { PC++; ADC(cpu, getImmediate(PC), 2); PC++; break; }
        case 0x65: { PC++; ADC(cpu, getZeroPage(PC), 3); PC++; break; }
        case 0x75: { PC++; ADC(cpu, getZeroPageX(PC), 4); PC++; break; }
        case 0x6D: { PC++; ADC(cpu, getAbsolute(PC), 4); PC += 2; break; }
        case 0x7D: { PC++; ADC(cpu, getAbsoluteX(PC), 4); PC += 2; break; }
        case 0x79: { PC++; ADC(cpu, getAbsoluteY(PC), 4); PC += 2; break; }
        case 0x61: { PC++; ADC(cpu, getIndirectX(PC), 6); PC++; break; }
        case 0x71: { PC++; ADC(cpu, getIndirectY(PC), 5); PC++; break; }

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
        case 0x0A: { PC++; ASL(cpu, getImmediate(PC), 2); break; }
        case 0x06: { PC++; ASL(cpu, getZeroPage(PC), 5); PC++; break; }
        case 0x16: { PC++; ASL(cpu, getZeroPageX(PC), 6); PC++; break; }
        case 0x0E: { PC++; ASL(cpu, getAbsolute(PC), 6); PC += 2; break; }
        case 0x1E: { PC++; ASL(cpu, getAbsoluteX(PC), 7); PC += 2; break; }

        // BCC
        case 0x90: { PC++; BCC(cpu); PC++; break; }

        // BCS
        case 0xB0: { PC++; BCS(cpu); PC++; break; }

        // BEQ
        case 0xF0: { PC++; BEQ(cpu); PC++; break; }

        // BIT
        case 0x24: { PC++; BIT(cpu, getZeroPage(PC), 3); PC++; break; }
        case 0x2C: { PC++; BIT(cpu, getAbsolute(PC), 4); PC += 2; break; }

        // BMI
        case 0x30: { PC++; BMI(cpu); PC++; break; }

        // BNE
        case 0xD0: { PC++; BNE(cpu); PC++; break; }

        // BPL
        case 0x10: { PC++; BPL(cpu); PC++; break; }

        // BVC
        case 0x50: { PC++; BVC(cpu); PC++; break; }

        // BVS
        case 0x70: { PC++; BVS(cpu); PC++; break; }

        // CLC
        case 0x18: { PC++; CLC(cpu); break; }

        // CLD
        case 0xD8: { PC++; CLD(cpu); break; }

        // CLI
        case 0x58: { PC++; CLI(cpu); break; }

        // CLV
        case 0xB8: { PC++; CLV(cpu); break; }

        // CMP
        case 0xC9: { PC++; CMP(cpu, getImmediate(PC), 2); PC++; break; }
        case 0xC5: { PC++; CMP(cpu, getZeroPage(PC), 3); PC++; break; }
        case 0xD5: { PC++; CMP(cpu, getZeroPageX(PC), 4); PC++; break; }
        case 0xCD: { PC++; CMP(cpu, getAbsolute(PC), 4); PC += 2; break; }
        case 0xDD: { PC++; CMP(cpu, getAbsoluteX(PC), 4); PC += 2; break; }
        case 0xD9: { PC++; CMP(cpu, getAbsoluteY(PC), 4); PC += 2; break; }
        case 0xC1: { PC++; CMP(cpu, getIndirectX(PC), 6); PC++; break; }
        case 0xD1: { PC++; CMP(cpu, getIndirectY(PC), 5); PC++; break; }

        // CPX
        case 0xE0: { PC++; CPX(cpu, getImmediate(PC), 2); PC++; break; }
        case 0xE4: { PC++; CPX(cpu, getZeroPage(PC), 3); PC++; break; }
        case 0xEC: { PC++; CPX(cpu, getAbsolute(PC), 4); PC += 2; break; }

        // CPY
        case 0xC0: { PC++; CPY(cpu, getImmediate(PC), 2); PC++; break; }
        case 0xC4: { PC++; CPY(cpu, getZeroPage(PC), 3); PC++; break; }
        case 0xCC: { PC++; CPY(cpu, getAbsolute(PC), 4); PC += 2; break; }

        // DEC
        case 0xC6: { PC++; DEC(cpu, getZeroPage(PC), 5); PC++; break; }
        case 0xD6: { PC++; DEC(cpu, getZeroPageX(PC), 6); PC++; break; }
        case 0xCE: { PC++; DEC(cpu, getAbsolute(PC), 6); PC += 2; break; }
        case 0xDE: { PC++; DEC(cpu, getAbsoluteX(PC), 7); PC += 2; break; }

        // DEX
        case 0xCA: { DEX(cpu); break; }

        // DEY
        case 0x88: { DEY(cpu); break; }

        // EOR
        case 0x49: { PC++; EOR(cpu, getImmediate(PC), 2); PC++; break; }
        case 0x45: { PC++; EOR(cpu, getZeroPage(PC), 3); PC++; break; }
        case 0x55: { PC++; EOR(cpu, getZeroPageX(PC), 4); PC++; break; }
        case 0x4D: { PC++; EOR(cpu, getAbsolute(PC), 4); PC += 2; break; }
        case 0x5D: { PC++; EOR(cpu, getAbsoluteX(PC), 4); PC += 2; break; }
        case 0x59: { PC++; EOR(cpu, getAbsoluteY(PC), 4); PC += 2; break; }
        case 0x41: { PC++; EOR(cpu, getIndirectX(PC), 6); PC++; break; }
        case 0x51: { PC++; EOR(cpu, getIndirectY(PC), 5); PC++; break; }

        // INC
        case 0xE6: { PC++; INC(cpu, getZeroPage(PC), 5); PC++; break; }
        case 0xF6: { PC++; INC(cpu, getZeroPageX(PC), 6); PC++; break; }
        case 0xEE: { PC++; INC(cpu, getAbsolute(PC), 6); PC += 2; break; }
        case 0xFE: { PC++; INC(cpu, getAbsoluteX(PC), 7); PC += 2; break; }

        // INX
        case 0xE8: { INX(cpu); break; }

        // INY
        case 0xC8: { INY(cpu); break; }

        // JMP
        case 0x4C: { PC++; JMP(cpu, getAbsolute(PC)); break; }
        case 0x6C: { PC++; JMP(cpu, getIndirectX(PC)); break; }

        // JSR
        case 0x20: { PC++; JSR(cpu, getAbsolute(PC)); break; }

        // LDA
        case 0xA9: { PC++; LDA(cpu, getImmediate(PC), 2); PC++; break; }
        case 0xA5: { PC++; LDA(cpu, getZeroPage(PC), 3); PC++; break; }
        case 0xB5: { PC++; LDA(cpu, getZeroPageX(PC), 4); PC++; break; }
        case 0xAD: { PC++; LDA(cpu, getAbsolute(PC), 4); PC += 2; break; }
        case 0xBD: { PC++; LDA(cpu, getAbsoluteX(PC), 4); PC += 2; break; }
        case 0xB9: { PC++; LDA(cpu, getAbsoluteY(PC), 4); PC += 2; break; }
        case 0xA1: { PC++; LDA(cpu, getIndirectX(PC), 6); PC++; break; }
        case 0xB1: { PC++; LDA(cpu, getIndirectY(PC), 5); PC++; break; }

        // LDX
        case 0xA2: { PC++; LDX(cpu, getImmediate(PC), 2); PC++; break; }
        case 0xA6: { PC++; LDX(cpu, getZeroPage(PC), 3); PC++; break; }
        case 0xB6: { PC++; LDX(cpu, getZeroPageY(PC), 4); PC++; break; }
        case 0xAE: { PC++; LDX(cpu, getAbsolute(PC), 4); PC += 2; break; }
        case 0xBE: { PC++; LDX(cpu, getAbsoluteY(PC), 4); PC += 2; break; }

        // LDY
        case 0xA0: { PC++; LDY(cpu, getImmediate(PC), 2); PC++; break; }
        case 0xA4: { PC++; LDY(cpu, getZeroPage(PC), 3); PC++; break; }
        case 0xB4: { PC++; LDY(cpu, getZeroPageX(PC), 4); PC++; break; }
        case 0xAC: { PC++; LDY(cpu, getAbsolute(PC), 4); PC += 2; break; }
        case 0xBC: { PC++; LDY(cpu, getAbsoluteX(PC), 4); PC += 2; break; }

        // LSR
        case 0x4A: { LSR(cpu, cpu->registers.acc, 2); break; }
        case 0x46: { PC++; LSR(cpu, getZeroPage(PC), 5); PC++; break; }
        case 0x56: { PC++; LSR(cpu, getZeroPageX(PC), 6); PC++; break; }
        case 0x4E: { PC++; LSR(cpu, getAbsolute(PC), 6); PC += 2; break; }
        case 0x5E: { PC++; LSR(cpu, getAbsoluteX(PC), 7); PC += 2; break; }

        // NOP
        case 0xEA: { NOP(cpu); break; }

        // ORA
        case 0x09: { PC++; ORA(cpu, getImmediate(PC), 2); PC++; break; }
        case 0x05: { PC++; ORA(cpu, getZeroPage(PC), 3); PC++; break; }
        case 0x15: { PC++; ORA(cpu, getZeroPageX(PC), 4); PC++; break; }
        case 0x0D: { PC++; ORA(cpu, getAbsolute(PC), 4); PC += 2; break; }
        case 0x1D: { PC++; ORA(cpu, getAbsoluteX(PC), 4); PC += 2; break; }
        case 0x19: { PC++; ORA(cpu, getAbsoluteY(PC), 4); PC += 2; break; }
        case 0x01: { PC++; ORA(cpu, getIndirectX(PC), 6); PC++; break; }
        case 0x11: { PC++; ORA(cpu, getIndirectY(PC), 5); PC++; break; }

        // PHA
        case 0x48: { PHA(cpu); break; }

        // PHP
        case 0x08: { PHP(cpu); break; }

        // PLA
        case 0x68: { PLA(cpu); break; }

        // PLP
        case 0x28: { PLP(cpu); break; }

        // ROL
        case 0x2A: { ROL(cpu, cpu->registers.acc, 2); break; }
        case 0x26: { PC++; ROL(cpu, getZeroPage(PC), 5); PC++; break; }
        case 0x36: { PC++; ROL(cpu, getZeroPageX(PC), 6); PC++; break; }
        case 0x2E: { PC++; ROL(cpu, getAbsolute(PC), 6); PC += 2; break; }
        case 0x3E: { PC++; ROL(cpu, getAbsoluteX(PC), 7); PC += 2; break; }

        // ROR
        case 0x6A: { ROR(cpu, cpu->registers.acc, 2); break; }
        case 0x66: { PC++; ROR(cpu, getZeroPage(PC), 5); PC++; break; }
        case 0x76: { PC++; ROR(cpu, getZeroPageX(PC), 6); PC++; break; }
        case 0x6E: { PC++; ROR(cpu, getAbsolute(PC), 6); PC += 2; break; }
        case 0x7E: { PC++; ROR(cpu, getAbsoluteX(PC), 7); PC += 2; break; }

        // RTI
        case 0x40: { RTI(cpu); break; }

        // RTS
        case 0x60: { RTS(cpu); break; }

        // SBC
        case 0xE9: { PC++; SBC(cpu, getImmediate(PC), 2); PC++; break; }
        case 0xE5: { PC++; SBC(cpu, getZeroPage(PC), 3); PC++; break; }
        case 0xF5: { PC++; SBC(cpu, getZeroPageX(PC), 4); PC++; break; }
        case 0xED: { PC++; SBC(cpu, getAbsolute(PC), 4); PC += 2; break; }
        case 0xFD: { PC++; SBC(cpu, getAbsoluteX(PC), 4); PC += 2; break; }
        case 0xF9: { PC++; SBC(cpu, getAbsoluteY(PC), 4); PC += 2; break; }
        case 0xE1: { PC++; SBC(cpu, getIndirectX(PC), 6); PC++; break; }
        case 0xF1: { PC++; SBC(cpu, getIndirectY(PC), 5); PC++; break; }

        // SEC
        case 0x38: { SEC(cpu); break; }

        // SED
        case 0xF8: { SED(cpu); break; }

        // SEI
        case 0x78: { SEI(cpu); break; }

        // STA
        case 0x85: { PC++; STA(cpu, getZeroPage(PC), 3); PC++; break; }
        case 0x95: { PC++; STA(cpu, getZeroPageX(PC), 4); PC++; break; }
        case 0x8D: { PC++; STA(cpu, getAbsolute(PC), 4); PC += 2; break; }
        case 0x9D: { PC++; STA(cpu, getAbsoluteX(PC), 5); PC += 2; break; }
        case 0x99: { PC++; STA(cpu, getAbsoluteY(PC), 5); PC += 2; break; }
        case 0x81: { PC++; STA(cpu, getIndirectX(PC), 6); PC++; break; }
        case 0x91: { PC++; STA(cpu, getIndirectY(PC), 6); PC++; break; }

        // STX
        case 0x86: { PC++; STX(cpu, getZeroPage(PC), 3); PC++; break; }
        case 0x96: { PC++; STX(cpu, getZeroPageY(PC), 4); PC++; break; }
        case 0x8E: { PC++; STX(cpu, getAbsolute(PC), 4); PC += 2; break; }

        // STY
        case 0x84: { PC++; STY(cpu, getZeroPage(PC), 3); PC++; break; }
        case 0x94: { PC++; STY(cpu, getZeroPageX(PC), 4); PC++; break; }
        case 0x8C: { PC++; STY(cpu, getAbsolute(PC), 4); PC += 2; break; }

        // TAX
        case 0xAA: { TAX(cpu); break; }

        // TAY
        case 0xA8: { TAY(cpu); break; }

        // TSX
        case 0xBA: { TSX(cpu); break; }

        // TXA
        case 0x8A: { TXA(cpu); break; }

        // TXS
        case 0x9A: { TXS(cpu); break; }

        // TYA
        case 0x98: { TYA(cpu); break; }

        default: { break; };
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