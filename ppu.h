#pragma once
#include <cstdint>
#include<iostream>
#include <memory>
#include "nes_cartridge.h"

const int PIXEL_COUNT = 256 * 240 * 3;  // NES window size times RGB.

class PPU2C02{

    public:
        PPU2C02();
        ~PPU2C02();

        // IO with main bus.
        void cpu_write(uint16_t addr, uint8_t data);
        uint8_t cpu_read(uint16_t addr, bool read_only);
        
        // IO with PPU bus.
        void ppu_write(uint16_t addr, uint8_t data);
        uint8_t ppu_read(uint16_t addr, bool read_only);

    public:
        // Interface with cartridge.
        void connect_cartridge(const std::shared_ptr<Cartridge>& cartridge);
        void clock();

    private:
        // Local pointer to cartridge as PPU can access a cartridge.
        std::shared_ptr<Cartridge> cart;

        int16_t scan_line = 0;  // Row on the screen.
        int16_t cycle = 0;  // Col on the screen.
        
    public:
        // This section stores what's being displayed.
        uint8_t pixels[PIXEL_COUNT] = {};
        uint8_t sprite_screen[PIXEL_COUNT] = {};
        uint8_t sprite_table[2][PIXEL_COUNT] = {};
        uint8_t pattern_table[2][128] = {};

        // Functions to access what's being displayed.
        uint8_t & get_sprite_screen();
        uint8_t & get_sprite_table(uint8_t i);
        uint8_t & get_pattern_table(uint8_t i);
        bool frame_complete = false;
        int ppu_frame_draw = 0;

    public:
        // Emulation related.
        bool is_emulation_run = false;  // True means run emulation in realtime.
        float residual_time = 0.0f;
};