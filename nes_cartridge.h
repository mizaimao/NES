#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

class Cartridge{

    public:
        Cartridge(const std::string& filename);

        // Conceptually the cartridge connects to both buses.
        // IO with main bus.
        void cpu_write(uint16_t addr, uint8_t data);
        uint8_t cpu_read(uint16_t addr, bool read_only);
        
        // IO with PPU bus.
        void ppu_write(uint16_t addr, uint8_t data);
        uint8_t ppu_read(uint16_t addr, bool read_only);

    private:
        // Cartridge memories.
        // Need vectors as we don't know how large the memory is until file is loaded.
        std::vector<uint8_t> PRG_memory;
        std::vector<uint8_t> CHR_memory;

};