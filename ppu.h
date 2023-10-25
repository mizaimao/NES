#pragma once
#include <cstdint>
#include<iostream>
#include <memory>
#include "nes_cartridge.h"

class PPU2C02{

    public:
        // IO with main bus.
        void cpu_write(uint16_t addr, uint8_t data);
        uint8_t cpu_read(uint16_t addr, bool read_only = false);
        
        // IO with PPU bus.
        void ppu_write(uint16_t addr, uint8_t data);
        uint8_t ppu_read(uint16_t addr, bool read_only = false);

    public:
        // Interface with cartridge.
        void connect_cartridge(const std::shared_ptr<Cartridge>& cartridge);
        void clock();

    private:
        // Local pointer to cartridge as PPU can access a cartridge.
        std::shared_ptr<Cartridge> cart;
};