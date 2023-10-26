#include "nes_cartridge.h"

Cartridge::Cartridge(const std::string& filename){

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
    };
    Header header;

    std::ifstream ifs;
    ifs.open(filename, std::ifstream::binary);
    if (ifs.is_open()){
        // Read header.
        ifs.read((char*)&header, sizeof(header));

        // Next 512 bytes is for training.
        if (header.mapper1 & 0x04){
            ifs.seekg(512, std::ios_base::cur);
        }
        
        // int mapper_id = ((header.mapper2) >> 4) << 4) | (header.mapper1 >> 4);
        // auto mirror = (header.mapper1 & 0x01) ? 
    }
}

Cartridge::~Cartridge(){}

void Cartridge::cpu_write(uint16_t addr, uint8_t data){
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
uint8_t Cartridge::cpu_read(uint16_t addr, bool read_only = false){
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

// IO with PPU bus.
void Cartridge::ppu_write(uint16_t addr, uint8_t data){
    addr &= 0x3FFF;
}
uint8_t Cartridge::ppu_read(uint16_t addr, bool read_only = false){
    uint8_t data = 0x00;
    addr &= 0x3FFF;
    return data;
}