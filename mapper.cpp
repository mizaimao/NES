#include "mapper.h"

Mapper::Mapper(uint8_t PRG_banks_count, uint8_t CHR_banks_count){
    n_PRG_banks = PRG_banks_count;
    n_CHR_banks = CHR_banks_count;
};

Mapper::~Mapper(){};