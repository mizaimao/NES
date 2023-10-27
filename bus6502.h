#pragma once
#include <array>
#include <memory>
#include "cpu.h"
#include "ppu.cpp"
#include "nes_cartridge.cpp"

class Bus{
    // Devices connected to the Bus.
    public:
        // Structors mustn't present here.
        Bus();
        ~Bus();

        SY6502 cpu;
        std::array<uint8_t, 2048> cpu_ram;

        // Connect PPU to the bus.
        PPU2C02 ppu;

        // Connect cartridge to the bus.
        std::shared_ptr<Cartridge> cart = nullptr;

    private:
        // VRAM holding name table information.
        uint8_t name_table[2][1024];  // Physical memory on ppu (VRAM).
        uint8_t palette[32];    // Physical memory for palettes.
        // uint8_t pattern[2][4096];  // Placeholder for unknown usage.


    public:
        // Only cpu get to RW on/to bus.
        void cpu_write(uint16_t addr, uint8_t data);
        uint8_t cpu_read(uint16_t addr, bool read_only);

    public:
        // NES operations.
        void insert_cartridge(const std::shared_ptr<Cartridge>& cartridge);
        void reset();  // Reset button on a NES machine.
        void clock();  // Performs system ticks.

    public:  // Should have been private.
        uint32_t system_clock_counter = 0;
};