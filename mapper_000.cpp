#include "mapper_000.h"
#include <iostream>

Mapper_000::Mapper_000(uint8_t PRG_banks_count, uint8_t CHR_banks_count )
: Mapper(PRG_banks_count, CHR_banks_count){
};

Mapper_000::~Mapper_000(){};

bool Mapper_000::cpu_map_read(uint16_t addr, uint32_t &mapped_addr) {
    if (0x8000 <= addr and addr <= 0xFFFF){
        //std::cout << "mapper read cpu is called "<< std::endl;
        // https://www.nesdev.org/wiki/NROM section Banks. 16KiB cart have two banks and the second
        // half of address mirrors the first half.
        mapped_addr = addr & (n_PRG_banks > 1 ? 0x7FFF : 0x3FFF);
        return true;
    }
    return false;
};
bool Mapper_000::cpu_map_write(uint16_t addr, uint32_t &mapped_addr) {
    if (0x8000 <= addr and addr <= 0xFFFF){
        mapped_addr = addr & (n_PRG_banks > 1 ? 0x7FFF : 0x3FFF);
        return true;
    }
    return false;

};
bool Mapper_000::ppu_map_read(uint16_t addr, uint32_t &mapped_addr) {
    if (0x0000 <= addr and addr <= 0x1FFF){
        // For PPU there is not a bank switching. So no mapping and just copy the addr.
        mapped_addr = addr;
        return true;
    }
    return false;

};
bool Mapper_000::ppu_map_write(uint16_t addr, uint32_t &mapped_addr) {
    // PPU has read-only memory; so we don't do anything.

    // if (0x0000 <= addr and addr <= 0x1FFF){
    //     return true;
    // }
    return false;
};