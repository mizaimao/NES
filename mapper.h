#pragma once
#include <cstdint>


// ABC.
class Mapper{
    public:
        Mapper(uint8_t PRG_banks_count, uint8_t CHR_banks_count);
        ~Mapper();

    public:
        // Mapper does not provide data, but to translate address.
        virtual bool cpu_map_read(uint16_t addr, uint32_t &mapped_addr) = 0;
        virtual bool cpu_map_write(uint16_t addr, uint32_t &mapped_addr) = 0;
        virtual bool ppu_map_read(uint16_t addr, uint32_t &mapped_addr) = 0;
        virtual bool ppu_map_write(uint16_t addr, uint32_t &mapped_addr) = 0;

    protected:
        uint8_t n_PRG_banks;
        uint8_t n_CHR_banks;

};