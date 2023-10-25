// Credits to https://www.youtube.com/watch?v=8XmxKPJDGU0
//#pragma once
#include "cpu.h"
#include <vector>
#include <iostream>


SY6502::SY6502(){

    std::cout << "CPU constructed" << std::endl;
    // Add instruction table.
    using s = SY6502;
    instruction_table = {
        { "BRK", &s::BRK, &s::IMM, 7 },{ "ORA", &s::ORA, &s::IZX, 6 },{ "???", &s::XXX, &s::IMP, 2 },{ "???", &s::XXX, &s::IMP, 8 },{ "???", &s::NOP, &s::IMP, 3 },{ "ORA", &s::ORA, &s::ZP0, 3 },{ "ASL", &s::ASL, &s::ZP0, 5 },{ "???", &s::XXX, &s::IMP, 5 },{ "PHP", &s::PHP, &s::IMP, 3 },{ "ORA", &s::ORA, &s::IMM, 2 },{ "ASL", &s::ASL, &s::IMP, 2 },{ "???", &s::XXX, &s::IMP, 2 },{ "???", &s::NOP, &s::IMP, 4 },{ "ORA", &s::ORA, &s::ABS, 4 },{ "ASL", &s::ASL, &s::ABS, 6 },{ "???", &s::XXX, &s::IMP, 6 },
		{ "BPL", &s::BPL, &s::REL, 2 },{ "ORA", &s::ORA, &s::IZY, 5 },{ "???", &s::XXX, &s::IMP, 2 },{ "???", &s::XXX, &s::IMP, 8 },{ "???", &s::NOP, &s::IMP, 4 },{ "ORA", &s::ORA, &s::ZPX, 4 },{ "ASL", &s::ASL, &s::ZPX, 6 },{ "???", &s::XXX, &s::IMP, 6 },{ "CLC", &s::CLC, &s::IMP, 2 },{ "ORA", &s::ORA, &s::ABY, 4 },{ "???", &s::NOP, &s::IMP, 2 },{ "???", &s::XXX, &s::IMP, 7 },{ "???", &s::NOP, &s::IMP, 4 },{ "ORA", &s::ORA, &s::ABX, 4 },{ "ASL", &s::ASL, &s::ABX, 7 },{ "???", &s::XXX, &s::IMP, 7 },
		{ "JSR", &s::JSR, &s::ABS, 6 },{ "AND", &s::AND, &s::IZX, 6 },{ "???", &s::XXX, &s::IMP, 2 },{ "???", &s::XXX, &s::IMP, 8 },{ "BIT", &s::BIT, &s::ZP0, 3 },{ "AND", &s::AND, &s::ZP0, 3 },{ "ROL", &s::ROL, &s::ZP0, 5 },{ "???", &s::XXX, &s::IMP, 5 },{ "PLP", &s::PLP, &s::IMP, 4 },{ "AND", &s::AND, &s::IMM, 2 },{ "ROL", &s::ROL, &s::IMP, 2 },{ "???", &s::XXX, &s::IMP, 2 },{ "BIT", &s::BIT, &s::ABS, 4 },{ "AND", &s::AND, &s::ABS, 4 },{ "ROL", &s::ROL, &s::ABS, 6 },{ "???", &s::XXX, &s::IMP, 6 },
		{ "BMI", &s::BMI, &s::REL, 2 },{ "AND", &s::AND, &s::IZY, 5 },{ "???", &s::XXX, &s::IMP, 2 },{ "???", &s::XXX, &s::IMP, 8 },{ "???", &s::NOP, &s::IMP, 4 },{ "AND", &s::AND, &s::ZPX, 4 },{ "ROL", &s::ROL, &s::ZPX, 6 },{ "???", &s::XXX, &s::IMP, 6 },{ "SEC", &s::SEC, &s::IMP, 2 },{ "AND", &s::AND, &s::ABY, 4 },{ "???", &s::NOP, &s::IMP, 2 },{ "???", &s::XXX, &s::IMP, 7 },{ "???", &s::NOP, &s::IMP, 4 },{ "AND", &s::AND, &s::ABX, 4 },{ "ROL", &s::ROL, &s::ABX, 7 },{ "???", &s::XXX, &s::IMP, 7 },
		{ "RTI", &s::RTI, &s::IMP, 6 },{ "EOR", &s::EOR, &s::IZX, 6 },{ "???", &s::XXX, &s::IMP, 2 },{ "???", &s::XXX, &s::IMP, 8 },{ "???", &s::NOP, &s::IMP, 3 },{ "EOR", &s::EOR, &s::ZP0, 3 },{ "LSR", &s::LSR, &s::ZP0, 5 },{ "???", &s::XXX, &s::IMP, 5 },{ "PHA", &s::PHA, &s::IMP, 3 },{ "EOR", &s::EOR, &s::IMM, 2 },{ "LSR", &s::LSR, &s::IMP, 2 },{ "???", &s::XXX, &s::IMP, 2 },{ "JMP", &s::JMP, &s::ABS, 3 },{ "EOR", &s::EOR, &s::ABS, 4 },{ "LSR", &s::LSR, &s::ABS, 6 },{ "???", &s::XXX, &s::IMP, 6 },
		{ "BVC", &s::BVC, &s::REL, 2 },{ "EOR", &s::EOR, &s::IZY, 5 },{ "???", &s::XXX, &s::IMP, 2 },{ "???", &s::XXX, &s::IMP, 8 },{ "???", &s::NOP, &s::IMP, 4 },{ "EOR", &s::EOR, &s::ZPX, 4 },{ "LSR", &s::LSR, &s::ZPX, 6 },{ "???", &s::XXX, &s::IMP, 6 },{ "CLI", &s::CLI, &s::IMP, 2 },{ "EOR", &s::EOR, &s::ABY, 4 },{ "???", &s::NOP, &s::IMP, 2 },{ "???", &s::XXX, &s::IMP, 7 },{ "???", &s::NOP, &s::IMP, 4 },{ "EOR", &s::EOR, &s::ABX, 4 },{ "LSR", &s::LSR, &s::ABX, 7 },{ "???", &s::XXX, &s::IMP, 7 },
		{ "RTS", &s::RTS, &s::IMP, 6 },{ "ADC", &s::ADC, &s::IZX, 6 },{ "???", &s::XXX, &s::IMP, 2 },{ "???", &s::XXX, &s::IMP, 8 },{ "???", &s::NOP, &s::IMP, 3 },{ "ADC", &s::ADC, &s::ZP0, 3 },{ "ROR", &s::ROR, &s::ZP0, 5 },{ "???", &s::XXX, &s::IMP, 5 },{ "PLA", &s::PLA, &s::IMP, 4 },{ "ADC", &s::ADC, &s::IMM, 2 },{ "ROR", &s::ROR, &s::IMP, 2 },{ "???", &s::XXX, &s::IMP, 2 },{ "JMP", &s::JMP, &s::IND, 5 },{ "ADC", &s::ADC, &s::ABS, 4 },{ "ROR", &s::ROR, &s::ABS, 6 },{ "???", &s::XXX, &s::IMP, 6 },
		{ "BVS", &s::BVS, &s::REL, 2 },{ "ADC", &s::ADC, &s::IZY, 5 },{ "???", &s::XXX, &s::IMP, 2 },{ "???", &s::XXX, &s::IMP, 8 },{ "???", &s::NOP, &s::IMP, 4 },{ "ADC", &s::ADC, &s::ZPX, 4 },{ "ROR", &s::ROR, &s::ZPX, 6 },{ "???", &s::XXX, &s::IMP, 6 },{ "SEI", &s::SEI, &s::IMP, 2 },{ "ADC", &s::ADC, &s::ABY, 4 },{ "???", &s::NOP, &s::IMP, 2 },{ "???", &s::XXX, &s::IMP, 7 },{ "???", &s::NOP, &s::IMP, 4 },{ "ADC", &s::ADC, &s::ABX, 4 },{ "ROR", &s::ROR, &s::ABX, 7 },{ "???", &s::XXX, &s::IMP, 7 },
		{ "???", &s::NOP, &s::IMP, 2 },{ "STA", &s::STA, &s::IZX, 6 },{ "???", &s::NOP, &s::IMP, 2 },{ "???", &s::XXX, &s::IMP, 6 },{ "STY", &s::STY, &s::ZP0, 3 },{ "STA", &s::STA, &s::ZP0, 3 },{ "STX", &s::STX, &s::ZP0, 3 },{ "???", &s::XXX, &s::IMP, 3 },{ "DEY", &s::DEY, &s::IMP, 2 },{ "???", &s::NOP, &s::IMP, 2 },{ "TXA", &s::TXA, &s::IMP, 2 },{ "???", &s::XXX, &s::IMP, 2 },{ "STY", &s::STY, &s::ABS, 4 },{ "STA", &s::STA, &s::ABS, 4 },{ "STX", &s::STX, &s::ABS, 4 },{ "???", &s::XXX, &s::IMP, 4 },
		{ "BCC", &s::BCC, &s::REL, 2 },{ "STA", &s::STA, &s::IZY, 6 },{ "???", &s::XXX, &s::IMP, 2 },{ "???", &s::XXX, &s::IMP, 6 },{ "STY", &s::STY, &s::ZPX, 4 },{ "STA", &s::STA, &s::ZPX, 4 },{ "STX", &s::STX, &s::ZPY, 4 },{ "???", &s::XXX, &s::IMP, 4 },{ "TYA", &s::TYA, &s::IMP, 2 },{ "STA", &s::STA, &s::ABY, 5 },{ "TXS", &s::TXS, &s::IMP, 2 },{ "???", &s::XXX, &s::IMP, 5 },{ "???", &s::NOP, &s::IMP, 5 },{ "STA", &s::STA, &s::ABX, 5 },{ "???", &s::XXX, &s::IMP, 5 },{ "???", &s::XXX, &s::IMP, 5 },
		{ "LDY", &s::LDY, &s::IMM, 2 },{ "LDA", &s::LDA, &s::IZX, 6 },{ "LDX", &s::LDX, &s::IMM, 2 },{ "???", &s::XXX, &s::IMP, 6 },{ "LDY", &s::LDY, &s::ZP0, 3 },{ "LDA", &s::LDA, &s::ZP0, 3 },{ "LDX", &s::LDX, &s::ZP0, 3 },{ "???", &s::XXX, &s::IMP, 3 },{ "TAY", &s::TAY, &s::IMP, 2 },{ "LDA", &s::LDA, &s::IMM, 2 },{ "TAX", &s::TAX, &s::IMP, 2 },{ "???", &s::XXX, &s::IMP, 2 },{ "LDY", &s::LDY, &s::ABS, 4 },{ "LDA", &s::LDA, &s::ABS, 4 },{ "LDX", &s::LDX, &s::ABS, 4 },{ "???", &s::XXX, &s::IMP, 4 },
		{ "BCS", &s::BCS, &s::REL, 2 },{ "LDA", &s::LDA, &s::IZY, 5 },{ "???", &s::XXX, &s::IMP, 2 },{ "???", &s::XXX, &s::IMP, 5 },{ "LDY", &s::LDY, &s::ZPX, 4 },{ "LDA", &s::LDA, &s::ZPX, 4 },{ "LDX", &s::LDX, &s::ZPY, 4 },{ "???", &s::XXX, &s::IMP, 4 },{ "CLV", &s::CLV, &s::IMP, 2 },{ "LDA", &s::LDA, &s::ABY, 4 },{ "TSX", &s::TSX, &s::IMP, 2 },{ "???", &s::XXX, &s::IMP, 4 },{ "LDY", &s::LDY, &s::ABX, 4 },{ "LDA", &s::LDA, &s::ABX, 4 },{ "LDX", &s::LDX, &s::ABY, 4 },{ "???", &s::XXX, &s::IMP, 4 },
		{ "CPY", &s::CPY, &s::IMM, 2 },{ "CMP", &s::CMP, &s::IZX, 6 },{ "???", &s::NOP, &s::IMP, 2 },{ "???", &s::XXX, &s::IMP, 8 },{ "CPY", &s::CPY, &s::ZP0, 3 },{ "CMP", &s::CMP, &s::ZP0, 3 },{ "DEC", &s::DEC, &s::ZP0, 5 },{ "???", &s::XXX, &s::IMP, 5 },{ "INY", &s::INY, &s::IMP, 2 },{ "CMP", &s::CMP, &s::IMM, 2 },{ "DEX", &s::DEX, &s::IMP, 2 },{ "???", &s::XXX, &s::IMP, 2 },{ "CPY", &s::CPY, &s::ABS, 4 },{ "CMP", &s::CMP, &s::ABS, 4 },{ "DEC", &s::DEC, &s::ABS, 6 },{ "???", &s::XXX, &s::IMP, 6 },
		{ "BNE", &s::BNE, &s::REL, 2 },{ "CMP", &s::CMP, &s::IZY, 5 },{ "???", &s::XXX, &s::IMP, 2 },{ "???", &s::XXX, &s::IMP, 8 },{ "???", &s::NOP, &s::IMP, 4 },{ "CMP", &s::CMP, &s::ZPX, 4 },{ "DEC", &s::DEC, &s::ZPX, 6 },{ "???", &s::XXX, &s::IMP, 6 },{ "CLD", &s::CLD, &s::IMP, 2 },{ "CMP", &s::CMP, &s::ABY, 4 },{ "NOP", &s::NOP, &s::IMP, 2 },{ "???", &s::XXX, &s::IMP, 7 },{ "???", &s::NOP, &s::IMP, 4 },{ "CMP", &s::CMP, &s::ABX, 4 },{ "DEC", &s::DEC, &s::ABX, 7 },{ "???", &s::XXX, &s::IMP, 7 },
		{ "CPX", &s::CPX, &s::IMM, 2 },{ "SBC", &s::SBC, &s::IZX, 6 },{ "???", &s::NOP, &s::IMP, 2 },{ "???", &s::XXX, &s::IMP, 8 },{ "CPX", &s::CPX, &s::ZP0, 3 },{ "SBC", &s::SBC, &s::ZP0, 3 },{ "INC", &s::INC, &s::ZP0, 5 },{ "???", &s::XXX, &s::IMP, 5 },{ "INX", &s::INX, &s::IMP, 2 },{ "SBC", &s::SBC, &s::IMM, 2 },{ "NOP", &s::NOP, &s::IMP, 2 },{ "???", &s::SBC, &s::IMP, 2 },{ "CPX", &s::CPX, &s::ABS, 4 },{ "SBC", &s::SBC, &s::ABS, 4 },{ "INC", &s::INC, &s::ABS, 6 },{ "???", &s::XXX, &s::IMP, 6 },
		{ "BEQ", &s::BEQ, &s::REL, 2 },{ "SBC", &s::SBC, &s::IZY, 5 },{ "???", &s::XXX, &s::IMP, 2 },{ "???", &s::XXX, &s::IMP, 8 },{ "???", &s::NOP, &s::IMP, 4 },{ "SBC", &s::SBC, &s::ZPX, 4 },{ "INC", &s::INC, &s::ZPX, 6 },{ "???", &s::XXX, &s::IMP, 6 },{ "SED", &s::SED, &s::IMP, 2 },{ "SBC", &s::SBC, &s::ABY, 4 },{ "NOP", &s::NOP, &s::IMP, 2 },{ "???", &s::XXX, &s::IMP, 7 },{ "???", &s::NOP, &s::IMP, 4 },{ "SBC", &s::SBC, &s::ABX, 4 },{ "INC", &s::INC, &s::ABX, 7 },{ "???", &s::XXX, &s::IMP, 7 },
    };
}

SY6502::~SY6502(){
}
            
void SY6502::write(uint16_t addr, uint8_t data){
    bus -> write(addr, data);
};

uint8_t SY6502::read(uint16_t addr, bool read_only = false){
    return bus -> read(addr, read_only);
};

void SY6502::clock(){
    if (cycles == 0){
        // Read program counter and get one byte.
        // We then use this one byte to query the table.
        opcode = read(pc);
        pc++;

        // Then set remaining cycle to required cycle of this instruction.
        uint8_t base_cycle = instruction_table[opcode].cycles;

        // Call the function, including address mode and operation.
        // They may require additional cycles, and we add them to main cycle counter.
        uint8_t am_cycle = (this->*instruction_table[opcode].addrmode)();
        uint8_t op_cycle = (this->*instruction_table[opcode].operate)();

        cycles = base_cycle + am_cycle + op_cycle;
    }
    cycles--;
}

uint8_t SY6502::fetch(){
    // Exception is IMP because there is nothing to fetch.
    if (instruction_table[opcode].addrmode != &SY6502::IMP)
        fetched = read(addr_abs);
    return addr_abs;
}

// 12 Addressing modes.
// Implied.
uint8_t SY6502::IMP(){
    fetched = a;
    return 0;
}

// Immediate addressing. Get the second byte of program counter.
uint8_t SY6502::IMM(){
    addr_abs = pc++;
    return 0;
}

// Zero page addressing. Get only second byte of PC, and assuming the first
// byte is 0 (on page zero). 6502 programs tend to have instructions on the first
// page and this mode is faster than a full-address addressing. 
uint8_t SY6502::ZP0(){
    addr_abs = read(pc++);
    addr_abs &= 0x00FF;
    return 0;
}

// Zero page addressing with value in X/Y-register as offset.
uint8_t SY6502::ZPX(){
    addr_abs = (read(pc++) + x);
    addr_abs &= 0x00FF;
    return 0;
}
uint8_t SY6502::ZPY(){
    addr_abs = (read(pc++) + y);
    addr_abs &= 0x00FF;
    return 0;
}

// Absolute addressing. The next byte corresponds to the lower byte of address;
// while the second next byte is higher byte of address. When combined, they form
// a two-byte address.
uint8_t SY6502::ABS(){
    uint16_t low = read(pc++);
    uint16_t high = read(pc++);
    addr_abs = (high << 8) | low;
}

// Indexed absolute addressing with X and 
uint8_t SY6502::ABX(){
    uint16_t low = read(pc++);
    uint16_t high = read(pc++);
    addr_abs = (high << 8) | low;
    addr_abs += x;

    // Check if the new address is now on a different page.
    if ( (addr_abs & 0x00FF) != (high << 8))
        return 1;  // Additional cycle is needed.
    else
        return 0;

}
uint8_t SY6502::ABY(){
    uint16_t low = read(pc++);
    uint16_t high = read(pc++);
    addr_abs = (high << 8) | low;
    addr_abs += y;

    // Check if the new address is now on a different page.
    if ( (addr_abs & 0x00FF) != (high << 8))
        return 1;  // Additional cycle is needed.
    else
        return 0;
}

// Indirect indexed addressing. Current pc stores address to the next instruction.
// Hardware bug is also implemented.
uint8_t SY6502::IND(){
    uint16_t ptr_low = read(pc++);
    uint16_t ptr_high = read(pc++);
    uint16_t ptr = (ptr_high << 8) | ptr_low;
    // Hardware bug. When lower byte is 0xFF, the plus-one operation turns the page;
    // yet that was not the case in the real hardware.
    if (ptr_low == 0xFF)
        addr_abs = (read(ptr & 0xFF00) << 8) | read(ptr + 0);
    else
        addr_abs = (read(ptr + 1) << 8) | read(ptr + 0);

    return 0;
}

// Indirect addressing on zero page with X offset.
uint8_t SY6502::IZX(){
    uint16_t t = read(pc++);
    uint16_t low = read((uint16_t)(t + (uint16_t)x) & 0x00FF);
    uint16_t high = read((uint16_t)(t + (uint16_t)x + 1) & 0x00FF);

    addr_abs = (high << 8) | low;
    return 0;
};
// Indirect addressing on zero page with Y offset. This is NOT a complementary to X.
uint8_t SY6502::IZY(){
    uint16_t t = read(pc++);
    uint16_t low = read(t & 0x00FF);
    uint16_t high = read((t + 1) & 0x00FF);

    addr_abs = (high << 8) | low;
    addr_abs += y;

    if ((addr_abs & 0xFF00) != (high << 8))  // Page turned.
        return 1;
    return 0;
};

// Relative addressing mode. Used only in branching, range is minus and plus 128 bytes.
// The read value can be negative (unsigned)
uint8_t SY6502::REL(){
    addr_rel = read(pc++);

    if (addr_rel & 0x80)  // Signed numbers have bit7 (first bit) set to 1.
        addr_rel |= 0xFF00;  // It's signed, set high byte of relative address to 11.
    return 0;
};

// 56 instructions.
// Bit-wise logic "and".
uint8_t SY6502::AND(){
    fetch();
    a = a & fetched;
    set_flag(Z, a == 0x00);
    set_flag(N, a & 0x00);
}