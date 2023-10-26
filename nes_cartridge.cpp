#include "nes_cartridge.h"
#include "mapper_000.cpp"

Cartridge::Cartridge(const std::string& filename){

    std::ifstream ifs;
    ifs.open(filename, std::ifstream::binary);
    if (ifs.is_open()){
        // Read header.
        ifs.read((char*)&header, sizeof(header));

        // Next 512 bytes is for training.
        if (header.mapper1 & 0x04){
            ifs.seekg(512, std::ios_base::cur);
        }
        
        // Determine which mapper is being used.
        uint8_t mapper_id = ((header.mapper2 >> 4) << 4) | (header.mapper1 >> 4);
        auto mirror = (header.mapper1 & 0x01);

        // Get one of three NES file types.
        uint8_t nes_file_type = 1;

        if (nes_file_type == 0){}

        if (nes_file_type == 1){
            n_PRG_banks = header.prg_rom_chunks;
            PRG_memory.resize(n_PRG_banks * 16384);
            ifs.read((char*)PRG_memory.data(), PRG_memory.size());

            n_CHR_banks = header.chr_rom_chunks;
            CHR_memory.resize(n_CHR_banks * 8192);
            ifs.read((char*)CHR_memory.data(), CHR_memory.size());
        }

        if (nes_file_type == 2){}
        
        // Now get the the mappers.
        switch (mapper_id){
            case 0: p_mapper = std::make_shared<Mapper_000>(n_PRG_banks, n_CHR_banks); break;
        }

    }
};

Cartridge::~Cartridge(){};

bool Cartridge::cpu_write(uint16_t addr, uint8_t data){
    // Holding translated address.
    uint32_t mapped_addr = 0;
    // mapper.cpu_map_read tests if the data need to come from cartridge memory.
    if (p_mapper->cpu_map_read(addr, mapped_addr)){
        // If it does, then we read data from cartridge. Here mapped_addr is 
        // transformed by mapper and stores offset which we use to retrieve data.
        PRG_memory[mapped_addr] = data;
        return true;  // Cartridge handled this write operation.
    }
    return false;
}

bool Cartridge::cpu_read(uint16_t addr, bool read_only = false){
    uint32_t mapped_addr = 0;
    if (p_mapper->cpu_map_write(addr, mapped_addr)){
        uint8_t data = PRG_memory[mapped_addr];
        return true;
    }
    return false;
}

// IO with PPU bus.
// Different from cpu write, this set interfaces with Character memory.
bool Cartridge::ppu_write(uint16_t addr, uint8_t data){
    uint32_t mapped_addr = 0;
    if (p_mapper->cpu_map_write(addr, mapped_addr)){
        CHR_memory[mapped_addr] = data;
        return true;
    }
    return false;
}

bool Cartridge::ppu_read(uint16_t addr, bool read_only = false){
    uint32_t mapped_addr = 0;
    if (p_mapper->cpu_map_write(addr, mapped_addr)){
        uint8_t data = CHR_memory[mapped_addr];
        return true;
    }
    return false;
}