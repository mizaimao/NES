#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "mapper.h"

class Cartridge{

    public:
        Cartridge(const std::string& filename);
        ~Cartridge();

        // Header for iNES format.
        struct Header{
            char name[4];
            uint8_t prg_rom_chunks;
            uint8_t chr_rom_chunks;
            uint8_t mapper1;
            uint8_t mapper2;
            uint8_t prg_ram_size;
            uint8_t tv_system1;
            uint8_t tv_system2;
            char unused[5];
        } header;

        // Conceptually the cartridge connects to both buses.
        // IO with main bus.
        bool cpu_write(uint16_t addr, uint8_t data);
        bool cpu_read(uint16_t addr, bool read_only);
        
        // IO with PPU bus.
        bool ppu_write(uint16_t addr, uint8_t data);
        bool ppu_read(uint16_t addr, bool read_only);


    private:
        // Cartridge memories.
        // Need vectors as we don't know how large the memory is until file is loaded.
        std::vector<uint8_t> PRG_memory;
        std::vector<uint8_t> CHR_memory;

        std::shared_ptr<Mapper> p_mapper;

        uint8_t mapper_id = 0;
        uint8_t PRG_bank_id = 0;

        // Should be private;
        uint8_t n_CHR_banks = 0;
        uint8_t n_PRG_banks = 0;

};

Mapper::Mapper(uint8_t PRG_banks_count, uint8_t CHR_banks_count){
    n_PRG_banks = PRG_banks_count;
    n_CHR_banks = CHR_banks_count;
};

Mapper::~Mapper(){};