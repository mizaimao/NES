#include <array>
#include "bus.h"

Bus::Bus(){
        // // Init ram values.
        // std::cout << "Bus constructed" << std::endl;
        // for (auto &i : ram) i = 0x00;
        // // Connect the cpu to this bus.
        // cpu.connect_bus(this);
    };

Bus::~Bus(){
};
    
void Bus::write(uint16_t addr, uint8_t data){
    if (0x0000 <= addr and addr <= 0xFFFF){
        ram[addr] = data;
    }
};

uint8_t Bus::read(uint16_t addr, bool read_only = false){
    if (0x0000 <= addr and addr <= 0xFFFF){
        return ram[addr];
    }
    return 0x00;
};
