#pragma once

#include "common.h"

using std::unique_ptr;

// NES Base that contains instances of all other components
class NES {
public:
    NES() {
        cpu = std::make_unique<CPU>();
    }
    ~NES();
private:
    // NES Components
    unique_ptr<CPU> cpu;
    // PPU ppu;
    // APU apu;
    // Cartridge cartridge;
    // Controller controller1;
    // Controller controller2;
    // RAM ram;
    // ROM rom;
};