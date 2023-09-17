#include "cpu.h"

std::function<void()> CPU::getOpcodeFunction(u8 opcode) {
    switch (opcode) {
        case 0x00: return std::bind(&CPU::BRK, this, AddressingMode::IMPLIED);
        case 0x01: return std::bind(&CPU::ORA, this, AddressingMode::INDIRECT_X);
        case 0x05: return std::bind(&CPU::ORA, this, AddressingMode::ZERO_PAGE);
        case 0x06: return std::bind(&CPU::ASL, this, AddressingMode::ZERO_PAGE);
        case 0x08: return std::bind(&CPU::PHP, this, AddressingMode::IMPLIED);
        case 0x09: return std::bind(&CPU::ORA, this, AddressingMode::IMMEDIATE);
        case 0x0A: return std::bind(&CPU::ASL, this, AddressingMode::ACCUMULATOR);
        case 0x0D: return std::bind(&CPU::ORA, this, AddressingMode::ABSOLUTE);
        case 0x0E: return std::bind(&CPU::ASL, this, AddressingMode::ABSOLUTE);
        case 0x10: return std::bind(&CPU::BPL, this, AddressingMode::RELATIVE);
        case 0x11: return std::bind(&CPU::ORA, this, AddressingMode::INDIRECT_Y);
        case 0x15: return std::bind(&CPU::ORA, this, AddressingMode::ZERO_PAGE_X);
        case 0x16: return std::bind(&CPU::ASL, this, AddressingMode::ZERO_PAGE_X);
        case 0x18: return std::bind(&CPU::CLC, this, AddressingMode::IMPLIED);
        case 0x19: return std::bind(&CPU::ORA, this, AddressingMode::ABSOLUTE_Y);
        case 0x1D: return std::bind(&CPU::ORA, this, AddressingMode::ABSOLUTE_X);
        case 0x1E: return std::bind(&CPU::ASL, this, AddressingMode::ABSOLUTE_X);
        case 0x20: return std::bind(&CPU::JSR, this, AddressingMode::ABSOLUTE);
        case 0x21: return std::bind(&CPU::AND, this, AddressingMode::INDIRECT_X);
        case 0x24: return std::bind(&CPU::BIT, this, AddressingMode::ZERO_PAGE);
        case 0x25: return std::bind(&CPU::AND, this, AddressingMode::ZERO_PAGE);
        case 0x26: return std::bind(&CPU::ROL, this, AddressingMode::ZERO_PAGE);
        case 0x28: return std::bind(&CPU::PLP, this, AddressingMode::IMPLIED);
        case 0x29: return std::bind(&CPU::AND, this, AddressingMode::IMMEDIATE);
        case 0x2A: return std::bind(&CPU::ROL, this, AddressingMode::ACCUMULATOR);
        case 0x2C: return std::bind(&CPU::BIT, this, AddressingMode::ABSOLUTE);
        case 0x2D: return std::bind(&CPU::AND, this, AddressingMode::ABSOLUTE);
        case 0x2E: return std::bind(&CPU::ROL, this, AddressingMode::ABSOLUTE);
        case 0x30: return std::bind(&CPU::BMI, this, AddressingMode::RELATIVE);
        case 0x31: return std::bind(&CPU::AND, this, AddressingMode::INDIRECT_Y);
        case 0x35: return std::bind(&CPU::AND, this, AddressingMode::ZERO_PAGE_X);
        case 0x36: return std::bind(&CPU::ROL, this, AddressingMode::ZERO_PAGE_X);
        case 0x38: return std::bind(&CPU::SEC, this, AddressingMode::IMPLIED);
        case 0x39: return std::bind(&CPU::AND, this, AddressingMode::ABSOLUTE_Y);
        case 0x3D: return std::bind(&CPU::AND, this, AddressingMode::ABSOLUTE_X);
        case 0x3E: return std::bind(&CPU::ROL, this, AddressingMode::ABSOLUTE_X);
        case 0x40: return std::bind(&CPU::RTI, this, AddressingMode::IMPLIED);
        case 0x41: return std::bind(&CPU::EOR, this, AddressingMode::INDIRECT_X);
        case 0x45: return std::bind(&CPU::EOR, this, AddressingMode::ZERO_PAGE);
        case 0x46: return std::bind(&CPU::LSR, this, AddressingMode::ZERO_PAGE);
        case 0x48: return std::bind(&CPU::PHA, this, AddressingMode::IMPLIED);
        case 0x49: return std::bind(&CPU::EOR, this, AddressingMode::IMMEDIATE);
        case 0x4A: return std::bind(&CPU::LSR, this, AddressingMode::ACCUMULATOR);
        case 0x4C: return std::bind(&CPU::JMP, this, AddressingMode::ABSOLUTE);
        case 0x4D: return std::bind(&CPU::EOR, this, AddressingMode::ABSOLUTE);
        case 0x4E: return std::bind(&CPU::LSR, this, AddressingMode::ABSOLUTE);
        case 0x50: return std::bind(&CPU::BVC, this, AddressingMode::RELATIVE);
        case 0x51: return std::bind(&CPU::EOR, this, AddressingMode::INDIRECT_Y);
        case 0x55: return std::bind(&CPU::EOR, this, AddressingMode::ZERO_PAGE_X);
        case 0x56: return std::bind(&CPU::LSR, this, AddressingMode::ZERO_PAGE_X);
        case 0x58: return std::bind(&CPU::CLI, this, AddressingMode::IMPLIED);
        case 0x59: return std::bind(&CPU::EOR, this, AddressingMode::ABSOLUTE_Y);
        case 0x5D: return std::bind(&CPU::EOR, this, AddressingMode::ABSOLUTE_X);
        case 0x5E: return std::bind(&CPU::LSR, this, AddressingMode::ABSOLUTE_X);
        case 0x60: return std::bind(&CPU::RTS, this, AddressingMode::IMPLIED);
        case 0x61: return std::bind(&CPU::ADC, this, AddressingMode::INDIRECT_X);
        case 0x65: return std::bind(&CPU::ADC, this, AddressingMode::ZERO_PAGE);
        case 0x66: return std::bind(&CPU::ROR, this, AddressingMode::ZERO_PAGE);
        case 0x68: return std::bind(&CPU::PLA, this, AddressingMode::IMPLIED);
        case 0x69: return std::bind(&CPU::ADC, this, AddressingMode::IMMEDIATE);
        case 0x6A: return std::bind(&CPU::ROR, this, AddressingMode::ACCUMULATOR);
        case 0x6C: return std::bind(&CPU::JMP, this, AddressingMode::INDIRECT);
        case 0x6D: return std::bind(&CPU::ADC, this, AddressingMode::ABSOLUTE);
        case 0x6E: return std::bind(&CPU::ROR, this, AddressingMode::ABSOLUTE);
        case 0x70: return std::bind(&CPU::BVS, this, AddressingMode::RELATIVE);
        case 0x71: return std::bind(&CPU::ADC, this, AddressingMode::INDIRECT_Y);
        case 0x75: return std::bind(&CPU::ADC, this, AddressingMode::ZERO_PAGE_X);
        case 0x76: return std::bind(&CPU::ROR, this, AddressingMode::ZERO_PAGE_X);
        case 0x78: return std::bind(&CPU::SEI, this, AddressingMode::IMPLIED);
        case 0x79: return std::bind(&CPU::ADC, this, AddressingMode::ABSOLUTE_Y);
        case 0x7D: return std::bind(&CPU::ADC, this, AddressingMode::ABSOLUTE_X);
        case 0x7E: return std::bind(&CPU::ROR, this, AddressingMode::ABSOLUTE_X);
        case 0x81: return std::bind(&CPU::STA, this, AddressingMode::INDIRECT_X);
        case 0x84: return std::bind(&CPU::STY, this, AddressingMode::ZERO_PAGE);
        case 0x85: return std::bind(&CPU::STA, this, AddressingMode::ZERO_PAGE);
        case 0x86: return std::bind(&CPU::STX, this, AddressingMode::ZERO_PAGE);
        case 0x88: return std::bind(&CPU::DEY, this, AddressingMode::IMPLIED);
        case 0x8A: return std::bind(&CPU::TXA, this, AddressingMode::IMPLIED);
        case 0x8C: return std::bind(&CPU::STY, this, AddressingMode::ABSOLUTE);
        case 0x8D: return std::bind(&CPU::STA, this, AddressingMode::ABSOLUTE);
        case 0x8E: return std::bind(&CPU::STX, this, AddressingMode::ABSOLUTE);
        case 0x90: return std::bind(&CPU::BCC, this, AddressingMode::RELATIVE);
        case 0x91: return std::bind(&CPU::STA, this, AddressingMode::INDIRECT_Y);
        case 0x94: return std::bind(&CPU::STY, this, AddressingMode::ZERO_PAGE_X);
        case 0x95: return std::bind(&CPU::STA, this, AddressingMode::ZERO_PAGE_X);
        case 0x96: return std::bind(&CPU::STX, this, AddressingMode::ZERO_PAGE_Y);
        case 0x98: return std::bind(&CPU::TYA, this, AddressingMode::IMPLIED);
        case 0x99: return std::bind(&CPU::STA, this, AddressingMode::ABSOLUTE_Y);
        case 0x9A: return std::bind(&CPU::TXS, this, AddressingMode::IMPLIED);
        case 0x9D: return std::bind(&CPU::STA, this, AddressingMode::ABSOLUTE_X);
        case 0xA0: return std::bind(&CPU::LDY, this, AddressingMode::IMMEDIATE);
        case 0xA1: return std::bind(&CPU::LDA, this, AddressingMode::INDIRECT_X);
        case 0xA2: return std::bind(&CPU::LDX, this, AddressingMode::IMMEDIATE);
        case 0xA4: return std::bind(&CPU::LDY, this, AddressingMode::ZERO_PAGE);
        case 0xA5: return std::bind(&CPU::LDA, this, AddressingMode::ZERO_PAGE);
        case 0xA6: return std::bind(&CPU::LDX, this, AddressingMode::ZERO_PAGE);
        case 0xA8: return std::bind(&CPU::TAY, this, AddressingMode::IMPLIED);
        case 0xA9: return std::bind(&CPU::LDA, this, AddressingMode::IMMEDIATE);
        case 0xAA: return std::bind(&CPU::TAX, this, AddressingMode::IMPLIED);
        case 0xAC: return std::bind(&CPU::LDY, this, AddressingMode::ABSOLUTE);
        case 0xAD: return std::bind(&CPU::LDA, this, AddressingMode::ABSOLUTE);
        case 0xAE: return std::bind(&CPU::LDX, this, AddressingMode::ABSOLUTE);
        case 0xB0: return std::bind(&CPU::BCS, this, AddressingMode::RELATIVE);
        case 0xB1: return std::bind(&CPU::LDA, this, AddressingMode::INDIRECT_Y);
        case 0xB4: return std::bind(&CPU::LDY, this, AddressingMode::ZERO_PAGE_X);
        case 0xB5: return std::bind(&CPU::LDA, this, AddressingMode::ZERO_PAGE_X);
        case 0xB6: return std::bind(&CPU::LDX, this, AddressingMode::ZERO_PAGE_Y);
        case 0xB8: return std::bind(&CPU::CLV, this, AddressingMode::IMPLIED);
        case 0xB9: return std::bind(&CPU::LDA, this, AddressingMode::ABSOLUTE_Y);
        case 0xBA: return std::bind(&CPU::TSX, this, AddressingMode::IMPLIED);
        case 0xBC: return std::bind(&CPU::LDY, this, AddressingMode::ABSOLUTE_X);
        case 0xBD: return std::bind(&CPU::LDA, this, AddressingMode::ABSOLUTE_X);
        case 0xBE: return std::bind(&CPU::LDX, this, AddressingMode::ABSOLUTE_Y);
        case 0xC0: return std::bind(&CPU::CPY, this, AddressingMode::IMMEDIATE);
        case 0xC1: return std::bind(&CPU::CMP, this, AddressingMode::INDIRECT_X);
        case 0xC4: return std::bind(&CPU::CPY, this, AddressingMode::ZERO_PAGE);
        case 0xC5: return std::bind(&CPU::CMP, this, AddressingMode::ZERO_PAGE);
        case 0xC6: return std::bind(&CPU::DEC, this, AddressingMode::ZERO_PAGE);
        case 0xC8: return std::bind(&CPU::INY, this, AddressingMode::IMPLIED);
        case 0xC9: return std::bind(&CPU::CMP, this, AddressingMode::IMMEDIATE);
        case 0xCA: return std::bind(&CPU::DEX, this, AddressingMode::IMPLIED);
        case 0xCC: return std::bind(&CPU::CPY, this, AddressingMode::ABSOLUTE);
        case 0xCD: return std::bind(&CPU::CMP, this, AddressingMode::ABSOLUTE);
        case 0xCE: return std::bind(&CPU::DEC, this, AddressingMode::ABSOLUTE);
        case 0xD0: return std::bind(&CPU::BNE, this, AddressingMode::RELATIVE);
        case 0xD1: return std::bind(&CPU::CMP, this, AddressingMode::INDIRECT_Y);
        case 0xD5: return std::bind(&CPU::CMP, this, AddressingMode::ZERO_PAGE_X);
        case 0xD6: return std::bind(&CPU::DEC, this, AddressingMode::ZERO_PAGE_X);
        case 0xD8: return std::bind(&CPU::CLD, this, AddressingMode::IMPLIED);
        case 0xD9: return std::bind(&CPU::CMP, this, AddressingMode::ABSOLUTE_Y);
        case 0xDD: return std::bind(&CPU::CMP, this, AddressingMode::ABSOLUTE_X);
        case 0xDE: return std::bind(&CPU::DEC, this, AddressingMode::ABSOLUTE_X);
        case 0xE0: return std::bind(&CPU::CPX, this, AddressingMode::IMMEDIATE);
        case 0xE1: return std::bind(&CPU::SBC, this, AddressingMode::INDIRECT_X);
        case 0xE4: return std::bind(&CPU::CPX, this, AddressingMode::ZERO_PAGE);
        case 0xE5: return std::bind(&CPU::SBC, this, AddressingMode::ZERO_PAGE);
        case 0xE6: return std::bind(&CPU::INC, this, AddressingMode::ZERO_PAGE);
        case 0xE8: return std::bind(&CPU::INX, this, AddressingMode::IMPLIED);
        case 0xE9: return std::bind(&CPU::SBC, this, AddressingMode::IMMEDIATE);
        case 0xEA: return std::bind(&CPU::NOP, this, AddressingMode::IMPLIED);
        case 0xEC: return std::bind(&CPU::CPX, this, AddressingMode::ABSOLUTE);
        case 0xED: return std::bind(&CPU::SBC, this, AddressingMode::ABSOLUTE);
        case 0xEE: return std::bind(&CPU::INC, this, AddressingMode::ABSOLUTE);
        case 0xF0: return std::bind(&CPU::BEQ, this, AddressingMode::RELATIVE);
        case 0xF1: return std::bind(&CPU::SBC, this, AddressingMode::INDIRECT_Y);
        case 0xF5: return std::bind(&CPU::SBC, this, AddressingMode::ZERO_PAGE_X);
        case 0xF6: return std::bind(&CPU::INC, this, AddressingMode::ZERO_PAGE_X);
        case 0xF8: return std::bind(&CPU::SED, this, AddressingMode::IMPLIED);
        case 0xF9: return std::bind(&CPU::SBC, this, AddressingMode::ABSOLUTE_Y);
        case 0xFD: return std::bind(&CPU::SBC, this, AddressingMode::ABSOLUTE_X);
        case 0xFE: return std::bind(&CPU::INC, this, AddressingMode::ABSOLUTE_X);
        default: return std::bind(&CPU::NOP, this, AddressingMode::IMPLIED);
    }
}

void CPU::executeOpcode(u8 opcode) {
    getOpcodeFunction(opcode)();
}

CPU::CPU() {
    // Set initial states for registers and flags
}

CPU::~CPU() {

}

void CPU::ADC(AddressingMode mode) {
    
}

void CPU::AND(AddressingMode mode) {

}

void CPU::ASL(AddressingMode mode) {

}

void CPU::BCC(AddressingMode mode) {

}

void CPU::BCS(AddressingMode mode) {

}

void CPU::BEQ(AddressingMode mode) {

}

void CPU::BIT(AddressingMode mode) {

}

void CPU::BMI(AddressingMode mode) {

}

void CPU::BNE(AddressingMode mode) {

}

void CPU::BPL(AddressingMode mode) {

}

void CPU::BRK(AddressingMode mode) {

}

void CPU::BVC(AddressingMode mode) {

}

void CPU::BVS(AddressingMode mode) {

}

void CPU::CLC(AddressingMode mode) {

}

void CPU::CLD(AddressingMode mode) {

}

void CPU::CLI(AddressingMode mode) {

}

void CPU::CLV(AddressingMode mode) {

}

void CPU::CMP(AddressingMode mode) {

}

void CPU::CPX(AddressingMode mode) {

}

void CPU::CPY(AddressingMode mode) {

}

void CPU::DEC(AddressingMode mode) {

}

void CPU::DEX(AddressingMode mode) {

}

void CPU::DEY(AddressingMode mode) {

}

void CPU::EOR(AddressingMode mode) {

}

void CPU::INC(AddressingMode mode) {

}

void CPU::INX(AddressingMode mode) {

}

void CPU::INY(AddressingMode mode) {

}

void CPU::JMP(AddressingMode mode) {

}

void CPU::JSR(AddressingMode mode) {

}

void CPU::LDA(AddressingMode mode) {

}

void CPU::LDX(AddressingMode mode) {

}

void CPU::LDY(AddressingMode mode) {

}

void CPU::LSR(AddressingMode mode) {

}

void CPU::NOP(AddressingMode mode) {

}

void CPU::ORA(AddressingMode mode) {

}

void CPU::PHA(AddressingMode mode) {

}

void CPU::PHP(AddressingMode mode) {

}

void CPU::PLA(AddressingMode mode) {

}

void CPU::PLP(AddressingMode mode) {

}

void CPU::ROL(AddressingMode mode) {

}

void CPU::ROR(AddressingMode mode) {

}

void CPU::RTI(AddressingMode mode) {

}

void CPU::RTS(AddressingMode mode) {

}

void CPU::SBC(AddressingMode mode) {

}

void CPU::SEC(AddressingMode mode) {

}

void CPU::SED(AddressingMode mode) {

}

void CPU::SEI(AddressingMode mode) {

}

void CPU::STA(AddressingMode mode) {

}

void CPU::STX(AddressingMode mode) {

}

void CPU::STY(AddressingMode mode) {

}

void CPU::TAX(AddressingMode mode) {

}

void CPU::TAY(AddressingMode mode) {

}

void CPU::TSX(AddressingMode mode) {

}

void CPU::TXA(AddressingMode mode) {

}

void CPU::TXS(AddressingMode mode) {

}

void CPU::TYA(AddressingMode mode) {

}