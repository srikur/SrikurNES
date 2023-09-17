#pragma once

#include "common.h"

// Addressing Modes
enum class AddressingMode {
    IMMEDIATE,
    ZERO_PAGE,
    ZERO_PAGE_X,
    ZERO_PAGE_Y,
    ABSOLUTE,
    ABSOLUTE_X,
    ABSOLUTE_Y,
    INDIRECT,
    INDIRECT_X,
    INDIRECT_Y,
    IMPLIED,
    ACCUMULATOR,
    RELATIVE
};

// NES CPU Emulator
class CPU {
public:
    CPU();
    ~CPU();

    // Function pointer getOpcodeFunction
    std::function<void()> getOpcodeFunction(u8 opcode);
    void executeOpcode(u8 opcode);
private:
    // CPU Instructions
    void ADC(AddressingMode mode);
    void AND(AddressingMode mode);
    void ASL(AddressingMode mode);
    void BCC(AddressingMode mode);
    void BCS(AddressingMode mode);
    void BEQ(AddressingMode mode);
    void BIT(AddressingMode mode);
    void BMI(AddressingMode mode);
    void BNE(AddressingMode mode);
    void BPL(AddressingMode mode);
    void BRK(AddressingMode mode);
    void BVC(AddressingMode mode);
    void BVS(AddressingMode mode);
    void CLC(AddressingMode mode);
    void CLD(AddressingMode mode);
    void CLI(AddressingMode mode);
    void CLV(AddressingMode mode);
    void CPX(AddressingMode mode);
    void CMP(AddressingMode mode);
    void CPY(AddressingMode mode);
    void DEC(AddressingMode mode);
    void DEX(AddressingMode mode);
    void DEY(AddressingMode mode);
    void EOR(AddressingMode mode);
    void INC(AddressingMode mode);
    void INX(AddressingMode mode);
    void INY(AddressingMode mode);
    void JMP(AddressingMode mode);
    void JSR(AddressingMode mode);
    void LDA(AddressingMode mode);
    void LDX(AddressingMode mode);
    void LDY(AddressingMode mode);
    void LSR(AddressingMode mode);
    void NOP(AddressingMode mode);
    void ORA(AddressingMode mode);
    void PHA(AddressingMode mode);
    void PHP(AddressingMode mode);
    void PLA(AddressingMode mode);
    void PLP(AddressingMode mode);
    void ROL(AddressingMode mode);
    void ROR(AddressingMode mode);
    void RTI(AddressingMode mode);
    void RTS(AddressingMode mode);
    void SBC(AddressingMode mode);
    void SEC(AddressingMode mode);
    void SED(AddressingMode mode);
    void SEI(AddressingMode mode);
    void STA(AddressingMode mode);
    void STX(AddressingMode mode);
    void STY(AddressingMode mode);
    void TAX(AddressingMode mode);
    void TAY(AddressingMode mode);
    void TSX(AddressingMode mode);
    void TXA(AddressingMode mode);
    void TXS(AddressingMode mode);
    void TYA(AddressingMode mode);

    // Registers
    u8 a; // Accumulator
    u8 x; // X Reg
    u8 y; // Y Reg
    u8 sp; // Stack Pointer
    u8 sr; // Status Register

    u16 pc; // Program Counter
};