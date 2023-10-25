#include <array>
#include "bus6502.h"

Bus::Bus(){
        // Init ram values.
        std::cout << "Bus constructed" << std::endl;
        for (auto &i : cpu_ram) i = 0x00;
        // Connect the cpu to this bus.
        cpu.connect_bus(this);
    };

Bus::~Bus(){
};
    
void Bus::cpu_write(uint16_t addr, uint8_t data){
    // NES has 2KB ram but addressable range is 8KB. Higher range is just mapped from the first 2KB.
    if (0x0000 <= addr and addr <= 0x1FFF){
        cpu_ram[addr & 0x07FF] = data;  // 0x07FF is 2048.
    } else if (0x2000 <= addr and addr <= 0x3FFF)
        ppu.ppu_write(addr & 0x0007, data);
};

uint8_t Bus::cpu_read(uint16_t addr, bool read_only = false){
    uint8_t data = 0x00;

    if (0x0000 <= addr and addr <= 0x1FFF){
        return cpu_ram[addr & 0x07FF];
    } else if (0x2000 <= addr and addr <= 0x3FFF)
        return ppu.ppu_read(addr & 0x0007, read_only);

    return data;
};

void Bus::insert_cartridge(const std::shared_ptr<Cartridge>& cartridge){
    this->cart = cartridge;
    ppu.connect_cartridge(cartridge);

}
// Reset button on a NES machine.
void Bus::reset(){
    cpu.reset();
    system_clock_counter = 0;
}
// Performs system tick.
void Bus::clock(){

}
