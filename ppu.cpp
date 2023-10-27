#include "ppu.h"
#include <iostream>

const int MAX_WIDTH = 256;
const int MAX_HEIGHT = 240;


PPU2C02::PPU2C02(){}

PPU2C02::~PPU2C02(){}

void PPU2C02::connect_cartridge(const std::shared_ptr<Cartridge>& cartridge){
    this->cart = cartridge;
}

void PPU2C02::clock(){
    cycle++;

    int draw_pos = scan_line * 256 * 3 + cycle * 3;

    if (0 <= draw_pos and draw_pos <= MAX_WIDTH * MAX_HEIGHT * 3) {

        uint8_t single_color = rand() % 255;
        sprite_screen[draw_pos] = single_color;
        sprite_screen[draw_pos + 1] = single_color;
        sprite_screen[draw_pos + 2] = single_color;

        //std::cout << "Drawing at loc " << std::to_string(draw_pos) << " with value " << std::to_string(sprite_screen[draw_pos]) << std::endl;

    }

    if (cycle >= 341){  // Love these magic number.
        cycle = 0;
        scan_line++;

        if (scan_line >= 261){
            scan_line = -1;  // Will be explained when writing PPU. 
            frame_complete = true;
        }
    }
}

// CPU can only address 8 different locations on PPU.
uint8_t PPU2C02::cpu_read(uint16_t addr, bool read_only = false){
    uint8_t data = 0x00;
    switch (addr){
    case 0x0000:  // Control.
        break;
    case 0x0001:  // Mask.
        break;
    case 0x0002:  // Status.
        break;
    case 0x0003:  // OAM address.
        break;
    case 0x0004:  // OAM Data.
        break;
    case 0x0005:  // Scroll.
        break;
    case 0x0006:  // PPU Address.
        break;
    case 0x0007:  // PPU Data.
        break;

    default:
        break;
    }
    return data;
}

void PPU2C02::cpu_write(uint16_t addr, uint8_t data){
        switch (addr){
        case 0x0000:  // Control.
            break;
        case 0x0001:  // Mask.
            break;
        case 0x0002:  // Status.
            break;
        case 0x0003:  // OAM address.
            break;
        case 0x0004:  // OAM Data.
            break;
        case 0x0005:  // Scroll.
            break;
        case 0x0006:  // PPU Address.
            break;
        case 0x0007:  // PPU Data.
            break;

        default:
            break;
        }
}

// IO with PPU bus. IO is somewhat intercepted by the cartridge first. This may be is for future improvement.
// Or just something I can delete later.
uint8_t PPU2C02::ppu_read(uint16_t addr, bool read_only = false){
    uint8_t data = 0x00;
    // Masking some addresses to bound PPU's address range.
    addr &= 0x3FFF;

    if (cart != nullptr and cart -> ppu_read(addr, read_only)){
    }

    return data;
}

void PPU2C02::ppu_write(uint16_t addr, uint8_t data){
    addr &= 0x3FFF;
    if (cart != nullptr and cart -> ppu_write(addr, data)){
    }
}

uint8_t & PPU2C02::get_sprite_screen(){
    return *sprite_screen;
}

uint8_t & PPU2C02::get_sprite_table(uint8_t i){
    return *sprite_table[i];
}

uint8_t & PPU2C02::get_pattern_table(uint8_t i){
    return *pattern_table[i];
}

