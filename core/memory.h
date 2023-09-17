#pragma once

#include "common.h"

class Memory {
public:
    void writeByte(u16 address, u8 data);
    u8 readByte(u16 address);
private:
    // 2KB Internal RAM
    u8 ram[0x800];
};