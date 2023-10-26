#pragma once
#include "mapper.h"

class Mapper_000 : public Mapper{
    public:
        Mapper_000(uint8_t PRG_banks_count, uint8_t CHR_banks_count);
        ~Mapper_000();

    public:
        bool cpu_map_read(uint16_t addr, uint32_t &mapped_addr) override;
        bool cpu_map_write(uint16_t addr, uint32_t &mapped_addr) override;
        bool ppu_map_read(uint16_t addr, uint32_t &mapped_addr) override;
        bool ppu_map_write(uint16_t addr, uint32_t &mapped_addr) override;
};
