//#pragma once
#include "cpu.h"
#include <array>

class Bus{
    // Devices connected to the Bus.
    public:
        // Structors mustn't present here.
        // Bus();
        // ~Bus();

        SY6502 cpu;
        std::array<uint8_t, 64 * 1024> ram;

    public:
        void write(uint16_t addr, uint8_t data);
        uint8_t read(uint16_t addr, bool read_only = false);
};