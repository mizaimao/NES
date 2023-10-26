#include "ppu.h"

PPU2C02::PPU2C02(){}

PPU2C02::~PPU2C02(){}

void PPU2C02::connect_cartridge(const std::shared_ptr<Cartridge>& cartridge){
    this->cart = cartridge;
}

void PPU2C02::clock(){

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

