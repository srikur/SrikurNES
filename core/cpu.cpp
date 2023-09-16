#include "cpu.h"

std::function<void()> CPU::getOpcodeFunction(u8 opcode) {
    switch (opcode) {
        case 0x00: return &CPU::BRK;
        case 0x01: return &CPU::ORA;
        case 0x05: return &CPU::ORA;
        case 0x06: return &CPU::ASL;
        case 0x08: return &CPU::PHP;
        case 0x09: return &CPU::ORA;
        case 0x0A: return &CPU::ASL;
        case 0x0D: return &CPU::ORA;
        case 0x0E: return &CPU::ASL;
        case 0x10: return &CPU::BPL;
        case 0x11: return &CPU::ORA;
        case 0x15: return &CPU::ORA;
        case 0x16: return &CPU::ASL;
        case 0x18: return &CPU::CLC;
        case 0x19: return &CPU::ORA;
        case 0x1D: return &CPU::ORA;
        case 0x1E: return &CPU::ASL;
        case 0x20: return &CPU::JSR;
        case 0x21: return &CPU::AND;
        case 0x24: return &CPU::BIT;
        case 0x25: return &CPU::AND;
        case 0x26: return &CPU::ROL;
        case 0x28: return &CPU::PLP;
        case 0x29: return &CPU::AND;
        case 0x2A: return &CPU::ROL;
        case 0x2C: return &CPU::BIT;
        case 0x2D: return &CPU::AND;
        case 0x2E: return &CPU::ROL;
        case 0x30: return &CPU::BMI;
        case 0x31: return &CPU::AND;
        case 0x35: return &CPU::AND;
        case 0x36: return &CPU::ROL;
        case 0x38: return &CPU::SEC;
        case 0x39: return &CPU::AND;
        case 0x3D: return &CPU::AND;
        case 0x3E: return &CPU::ROL;
        case 0x40: return &CPU::RTI;
        case 0x41: return &CPU::EOR;
        case 0x45: return &CPU::EOR;
        case 0x46: return &CPU::LSR;
        case 0x48: return &CPU::PHA;
        case 0x49: return &CPU::EOR;
        case 0x4A: return &CPU::LSR;
        case 0x4C: return &CPU::JMP;
        case 0x4D: return &CPU::EOR;
        case 0x4E: return &CPU::LSR;
        case 0x50: return &CPU::BVC;
        case 0x51: return &CPU::EOR;
        case 0x55: return &CPU::EOR;
        case 0x56: return &CPU::LSR;
        case 0x58: return &CPU::CLI;
        case 0x59: return &CPU::EOR;
        case 0x5D: return &CPU::EOR;
        case 0x5E: return &CPU::LSR;
        case 0x60: return &CPU::RTS;
        case 0x61: return &CPU::ADC;
        case 0x65: return &CPU::ADC;
        case 0x66: return &CPU::ROR;
        case 0x68: return &CPU::PLA;
        case 0x69: return &CPU::ADC;
        case 0x6A: return &CPU::ROR;
        case 0x6C: return &CPU::JMP;
        case 0x6D: return &CPU::ADC;
        case 0x6E: return &CPU::ROR;
        case 0x70: return &CPU::BVS;
        case 0x71: return &CPU::ADC;
        case 0x75: return &CPU::ADC;
        case 0x76: return &CPU::ROR;
        case 0x78: return &CPU::SEI;
        case 0x79: return &CPU::ADC;
        case 0x7D: return &CPU::ADC;
        case 0x7E: return &CPU::ROR;
        case 0x81: return &CPU::STA;
        case 0x84: return &CPU::STY;
        case 0x85: return &CPU::STA;
        case 0x86: return &CPU::STX;
        case 0x88: return &CPU::DEY;
        case 0x8A: return &CPU::TXA;
        case 0x8C: return &CPU::STY;
        case 0x8D: return &CPU::STA;
        case 0x8E: return &CPU::STX;
        case 0x90: return &CPU::BCC;
        case 0x91: return &CPU::STA;
        case 0x94: return &CPU::STY;
        case 0x95: return &CPU::STA;
        case 0x96: return &CPU::STX;
        case 0x98: return &CPU::TYA;
        case 0x99: return &CPU::STA;
        case 0x9A: return &CPU::TXS;
        case 0x9D: return &CPU::STA;
        case 0xA0: return &CPU::LDY;
        case 0xA1: return &CPU::LDA;
        case 0xA2: return &CPU::LDX;
        case 0xA4: return &CPU::LDY;
        case 0xA5: return &CPU::LDA;
        case 0xA6: return &CPU::LDX;
        case 0xA8: return &CPU::TAY;
        case 0xA9: return &CPU::LDA;
        case 0xAA: return &CPU::TAX;
        case 0xAC: return &CPU::LDY;
        case 0xAD: return &CPU::LDA;
        case 0xAE: return &CPU::LDX;
        case 0xB0: return &CPU::BCS;
        case 0xB1: return &CPU::LDA;
        case 0xB4: return &CPU::LDY;
        case 0xB5: return &CPU::LDA;
        case 0xB6: return &CPU::LDX;
        case 0xB8: return &CPU::CLV;
        case 0xB9: return &CPU::LDA;
        case 0xBA: return &CPU::TSX;
        case 0xBC: return &CPU::LDY;
        case 0xBD: return &CPU::LDA;
        case 0xBE: return &CPU::LDX;
        case 0xC0: return &CPU::CPY;
        case 0xC1: return &CPU::CMP;
        case 0xC4: return &CPU::CPY;
        case 0xC5: return &CPU::CMP;
        case 0xC6: return &CPU::DEC;
        case 0xC8: return &CPU::INY;
        case 0xC9: return &CPU::CMP;
        case 0xCA: return &CPU::DEX;
        case 0xCC: return &CPU::CPY;
        case 0xCD: return &CPU::CMP;
        case 0xCE: return &CPU::DEC;
        case 0xD0: return &CPU::BNE;
        case 0xD1: return &CPU::CMP;
        case 0xD5: return &CPU::CMP;
        case 0xD6: return &CPU::DEC;
        case 0xD8: return &CPU::CLD;
        case 0xD9: return &CPU::CMP;
        case 0xDD: return &CPU::CMP;
        case 0xDE: return &CPU::DEC;
        case 0xE0: return &CPU::CPX;
        case 0xE1: return &CPU::SBC;
        case 0xE4: return &CPU::CPX;
        case 0xE5: return &CPU::SBC;
        case 0xE6: return &CPU::INC;
        case 0xE8: return &CPU::INX;
        case 0xE9: return &CPU::SBC;
        case 0xEA: return &CPU::NOP;
        case 0xEC: return &CPU::CPX;
        case 0xED: return &CPU::SBC;
        case 0xEE: return &CPU::INC;
        case 0xF0: return &CPU::BEQ;
        case 0xF1: return &CPU::SBC;
        case 0xF5: return &CPU::SBC;
        case 0xF6: return &CPU::INC;
        case 0xF8: return &CPU::SED;
        case 0xF9: return &CPU::SBC;
        case 0xFD: return &CPU::SBC;
        case 0xFE: return &CPU::INC;
        default: return &CPU::NOP;
    }
}

void CPU::executeOpcode(u8 opcode) {
    getOpcodeFunction(opcode)();
}

CPU::CPU() {
    
}

CPU::~CPU() {

}

CPU::ADC() {

}

CPU::AND() {

}

CPU::ASL() {

}

CPU::BCC() {

}

CPU::BCS() {

}

CPU::BEQ() {

}

CPU::BIT() {

}

CPU::BMI() {

}

CPU::BNE() {

}

CPU::BPL() {

}

CPU::BRK() {

}

CPU::BVC() {

}

CPU::BVS() {

}

CPU::CLC() {

}

CPU::CLD() {

}

CPU::CLI() {

}

CPU::CLV() {

}

CPU::CMP() {

}

CPU::CPX() {

}

CPU::CPY() {

}

CPU::DEC() {

}

CPU::DEX() {

}

CPU::DEY() {

}

CPU::EOR() {

}

CPU::INC() {

}

CPU::INX() {

}

CPU::INY() {

}

CPU::JMP() {

}

CPU::JSR() {

}

CPU::LDA() {

}

CPU::LDX() {

}

CPU::LDY() {

}

CPU::LSR() {

}

CPU::NOP() {

}

CPU::ORA() {

}

CPU::PHA() {

}

CPU::PHP() {

}

CPU::PLA() {

}

CPU::PLP() {

}

CPU::ROL() {

}

CPU::ROR() {

}

CPU::RTI() {

}

CPU::RTS() {

}

CPU::SBC() {

}

CPU::SEC() {

}

CPU::SED() {

}

CPU::SEI() {

}

CPU::STA() {

}

CPU::STX() {

}

CPU::STY() {

}

CPU::TAX() {

}

CPU::TAY() {

}

CPU::TSX() {

}

CPU::TXA() {

}

CPU::TXS() {

}

CPU::TYA() {

}