#include "ppu.h"

PPU2C02::PPU2C02(){}

PPU2C02::~PPU2C02(){}

void PPU2C02::connect_cartridge(const std::shared_ptr<Cartridge>& cartridge){
    this->cart = cartridge;
}

void PPU2C02::clock(){

}