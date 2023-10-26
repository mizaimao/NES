// Credits to https://www.youtube.com/watch?v=8XmxKPJDGU0
#pragma once
#include "cpu.h"
#include "bus6502.h"


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
    bus -> cpu_write(addr, data);
};

uint8_t SY6502::read(uint16_t addr, bool read_only = false){
    return bus -> cpu_read(addr, read_only);
};

// Disassembler function.
std::map<uint16_t, std::string> SY6502::disassemble(uint16_t start, uint16_t end){
    uint32_t addr = start;
    uint8_t value = 0x00;
    uint8_t low = 0x00;
    uint8_t high = 0x00;

    std::map<uint16_t, std::string> mapped_lines;
    uint16_t line_addr = 0x0000;

    // Utility stolen from the video to convert variable into hex string.
    auto hex = [](uint32_t n, uint8_t d)
	{
		std::string s(d, '0');
		for (int i = d - 1; i >= 0; i--, n >>= 4)
			s[i] = "0123456789ABCDEF"[n & 0xF];
		return s;
	};

    while (addr <= (uint32_t)end){


        line_addr = addr;
        // Generate instruction string.
        std::string instruction = "$" + hex(addr, 4) + ": ";
        
        // Get opcode and its name.
        uint8_t opcode = bus -> cpu_read(addr, true);
        addr ++;
        instruction += instruction_table[opcode].name + " ";

        if (instruction_table[opcode].addrmode == &SY6502::IMP){
            instruction += " {IMP}";
        }
        else if (instruction_table[opcode].addrmode == &SY6502::IMM){
            value = bus->cpu_read(addr, true); addr++;
            instruction += "#$" + hex(value, 2) + " {IMM}";
        }
        else if (instruction_table[opcode].addrmode == &SY6502::ZP0){
            low = bus->cpu_read(addr, true); addr++;
            high = 0x00;												
            instruction += "$" + hex(low, 2) + " {ZP0}";
        }
        else if (instruction_table[opcode].addrmode == &SY6502::ZPX){
            low = bus->cpu_read(addr, true); addr++;
            high = 0x00;														
            instruction += "$" + hex(low, 2) + ", X {ZPX}";
        }
        else if (instruction_table[opcode].addrmode == &SY6502::ZPY){
            low = bus->cpu_read(addr, true); addr++;
            high = 0x00;														
            instruction += "$" + hex(low, 2) + ", Y {ZPY}";
        }
        else if (instruction_table[opcode].addrmode == &SY6502::IZX){
            low = bus->cpu_read(addr, true); addr++;
            high = 0x00;								
            instruction += "($" + hex(low, 2) + ", X) {IZX}";
        }
        else if (instruction_table[opcode].addrmode == &SY6502::IZY){
            low = bus->cpu_read(addr, true); addr++;
            high = 0x00;								
            instruction += "($" + hex(low, 2) + "), Y {IZY}";
        }
        else if (instruction_table[opcode].addrmode == &SY6502::ABS){
            low = bus->cpu_read(addr, true); addr++;
            high = bus->cpu_read(addr, true); addr++;
            instruction += "$" + hex((uint16_t)(high << 8) | low, 4) + " {ABS}";
        }
        else if (instruction_table[opcode].addrmode == &SY6502::ABX){
            low = bus->cpu_read(addr, true); addr++;
            high = bus->cpu_read(addr, true); addr++;
            instruction += "$" + hex((uint16_t)(high << 8) | low, 4) + ", X {ABX}";
        }
        else if (instruction_table[opcode].addrmode == &SY6502::ABY){
            low = bus->cpu_read(addr, true); addr++;
            high = bus->cpu_read(addr, true); addr++;
            instruction += "$" + hex((uint16_t)(high << 8) | low, 4) + ", Y {ABY}";
        }
        else if (instruction_table[opcode].addrmode == &SY6502::IND){
            low = bus->cpu_read(addr, true); addr++;
            high = bus->cpu_read(addr, true); addr++;
            instruction += "($" + hex((uint16_t)(high << 8) | low, 4) + ") {IND}";
        }
        else if (instruction_table[opcode].addrmode == &SY6502::REL){
            value = bus->cpu_read(addr, true); addr++;
            instruction += "$" + hex(value, 2) + " [$" + hex(addr + (int8_t)value, 4) + "] {REL}";
        }
        mapped_lines[line_addr] = instruction;
    }
    return mapped_lines;
}
uint8_t SY6502::get_flag(FLAGS6502 f){
    return ((status & f) > 0) ? 1: 0;
}

void SY6502::set_flag(FLAGS6502 f, bool v){
    if (v)
        status |= f;
    else
        status &= ~f;
}

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

void SY6502::reset(){
    // Reset registers.
    a = 0;
    x = 0;
    y = 0;
    stkp = 0xFD;
    status = 0x00 | get_flag(U);
    // Reset addresses and PC.
    addr_abs = 0xFFFC;  // Initial address defined by 6502.
    uint16_t low = read(addr_abs + 0);
    uint16_t high = read(addr_abs + 1);
    pc = (high << 8) | low;
    // Reset helper variables.
    addr_rel = 0x0000;
    addr_abs = 0x0000;
    fetched = 0x00;

    cycles = 8;
}

void SY6502::irq(){
    if ( get_flag(I) == 0 ){  // Executes only if interrupt-allowing flag.
        write(0x0100 + stkp, (pc >> 8) & 0x00FF);
        stkp--;
        write(0x0100 + stkp, pc & 0x00FF);
        stkp--;

        set_flag(B, 0);  // Break flag.
        set_flag(U, 1);  // Not used, not sure why it's in the video.
        set_flag(I, 1);  // Irq disabled when set to 1.
        write(0x0100 + stkp, status);
        stkp--;

        // Hardcoded address to get next PC when interrupt occurs.
        addr_abs = 0xFFFE;
        uint16_t low = read(addr_abs + 0);
        uint16_t high = read(addr_abs + 1);
        pc = (high << 8) | low;

        cycles = 7;
    }
}

void SY6502::nmi(){
    write(0x0100 + stkp, (pc >> 8) & 0x00FF);
    stkp--;
    write(0x0100 + stkp, pc & 0x00FF);
    stkp--;

    set_flag(B, 0);  // Break flag.
    set_flag(U, 1);  // Not used, not sure why it's in the video.
    set_flag(I, 1);  // Irq disabled when set to 1.
    write(0x0100 + stkp, status);
    stkp--;

    // Hardcoded address to get next PC when interrupt occurs.
    addr_abs = 0xFFFA;  // Note this is different from irq.
    uint16_t low = read(addr_abs + 0);
    uint16_t high = read(addr_abs + 1);
    pc = (high << 8) | low;

    cycles = 8;
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
    return 0;
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

    return 1;
}
// Branch operations.
// Branch if carry bit is set. Can potentially require two additional cycles.
uint8_t SY6502::BCS(){
    if (get_flag(C) == 1){
        cycles++;  // Additional cycle needed if branches.
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00)){
            cycles ++;  // Additional cycle if page is crossed.
        }
        pc = addr_abs;
    }
    return 0;
}
// Brach on carry clear.
uint8_t SY6502::BCC(){
    if (get_flag(C) == 0){
        cycles++;  // Additional cycle needed if branches.
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00)){
            cycles ++;  // Additional cycle if page is crossed.
        }
        pc = addr_abs;
    }
    return 0;
}
// Branch on result zero.
uint8_t SY6502::BEQ(){
    if (get_flag(Z) == 1){
        cycles++;  // Additional cycle needed if branches.
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00)){
            cycles ++;  // Additional cycle if page is crossed.
        }
        pc = addr_abs;
    }
    return 0;
}
// Branch on result minus.
uint8_t SY6502::BMI(){
    if (get_flag(N) == 1){
        cycles++;  // Additional cycle needed if branches.
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00)){
            cycles ++;  // Additional cycle if page is crossed.
        }
        pc = addr_abs;
    }
    return 0;
}
// Branch on result not equal to zero.
uint8_t SY6502::BNE(){
    if (get_flag(Z) == 0){
        cycles++;  // Additional cycle needed if branches.
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00)){
            cycles ++;  // Additional cycle if page is crossed.
        }
        pc = addr_abs;
    }
    return 0;
}
// Branch on result plus (positive).
uint8_t SY6502::BPL(){
    if (get_flag(N) == 0){
        cycles++;  // Additional cycle needed if branches.
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00)){
            cycles ++;  // Additional cycle if page is crossed.
        }
        pc = addr_abs;
    }
    return 0;
}
// Branch on overflow.
uint8_t SY6502::BVS(){
    if (get_flag(V) == 1){
        cycles++;  // Additional cycle needed if branches.
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00)){
            cycles ++;  // Additional cycle if page is crossed.
        }
        pc = addr_abs;
    }
    return 0;
}
// Branch on overflow clear.
uint8_t SY6502::BVC(){
    if (get_flag(V) == 0){
        cycles++;  // Additional cycle needed if branches.
        addr_abs = pc + addr_rel;

        if ((addr_abs & 0xFF00) != (pc & 0xFF00)){
            cycles ++;  // Additional cycle if page is crossed.
        }
        pc = addr_abs;
    }
    return 0;
}

// Clear operations.
// Clear the carry flag.
uint8_t SY6502::CLC(){
    set_flag(C, false);
    return 0;
}
// Clear decimal mode.
uint8_t SY6502::CLD(){
    set_flag(D, false);
    return 0;
}
// Clear interrupt disable bit.
uint8_t SY6502::CLI(){
    set_flag(I, false);
    return 0;
}
// Clear overflow flag.
uint8_t SY6502::CLV(){
    set_flag(V, false);
    return 0;
}

// Addition. Add memory to accumulator with carry.
// Complexity arises when dealing with signed integers.
/**
 * Need to check two flags: negative and overflow.
 * Several scenarios when adding positive and negative numbers (signed):
 *     1. P + P may overflow;
 *     2. N + N may overflow;
 *     3. P + N will never overflow.
 * Therefore, the "V" register in flag register also helps when two signed
 * numbers are added.
 *  A truth table can help (0, 1 are the most significant bit of that
 *  number):
 *          A   M   R   V     A^R ~A^M    &
 *          0   0   0   0      0    1     0
 *          0   0   1   1      1    1     1
 *          0   1   0   0      0    0     0
 *          0   1   1   1      1    0     0
 *          1   0   0   0      1    0     0
 *          1   0   1   1      0    0     0
 *          1   1   0   0      1    1     1
 *          1   1   1   1      0    1     0 *
 *  where A is the number in accumulator, M is fetched number, and R is the
 *  result. V is the overflow flag, and A^R is exclusive-or.
 *  We can there are only two situations where overflow should be set, which
 *  can be calculated by using binary "and" between XOR(A, R) and ~XOR(A, M)
*/ 
uint8_t SY6502::ADC(){
    fetch();
    // Cast 8-bit values into 16-bit spaces.
    uint16_t temp = (uint16_t)a + (uint16_t)fetched + (uint16_t)get_flag(C);
    // Overflow can easily be checked in 16-bit space.
    set_flag(C, temp > 255);
    // Set zero flag if lower byte is zero.
    set_flag(Z, (temp & 0x00FF) == 0x00);
    // Negative flag is set by checking the most significant bit (7th bit).
    set_flag(N, temp & 0x80);
    // Overflow flag. Calculated by above table.
    set_flag(
        V,
        (~((uint16_t)a ^ (uint16_t)fetched) & ((uint16_t)a ^ (uint16_t)temp)) & 0x0080
    );
    // Save to accumulator and according to 6502 manual this requires an additional cycle.
    a = temp & 0x00FF;
    return 1;
}

// Subtraction. Subtract memory from accumulator with carry.
/**
 * Subtraction function. It updates accumulator with the result of
 * accumulator - fetched - (1 - carry); where the last item is the
 * opposite of carry bit, because in this case it's a "borrow" bit.
 *
 * It is wise for hardware designers to reuse existing implementation, and
 * in this case, we can convert this subtraction operation into addition.
 * We don't know if 6502 uses its addition module to perform subtraction.
 *
 * A = A - M - (1 - C) becomes A + (-1) * (M - (1 - C))
 * then, A = A + (- M) + 1 + C
 * and when flipping the sign of a signed number, we invert each bit and
 * plus one. E.g.
 * 5 = 0b00000101 and -5 = 0b11111010 + 0b00000001
 * Note that plus one operation can be included in the above deduced
 * formula, so for this subtraction operation, all we need to do is to
 * invert the number and use the addition function (ADC) codes.
 * This implementation reuses most part of addition function except one inversion operation.
*/
uint8_t SY6502::SBC(){
    fetch();
    uint16_t value = ((uint16_t)fetched) ^ 0x00FF;  // Inverts the lower byte using xor.
    uint16_t temp = (uint16_t)a + value + (uint16_t)get_flag(C);

    set_flag(C, temp > 255);  // Or temp & 0xFF00.
    set_flag(Z, (temp & 0x00FF) == 0x00);
    set_flag(N, temp & 0x80);
    set_flag(
        V,
        (((uint16_t)a ^ temp) & (value ^ temp)) & 0x0080
    );
    a = temp & 0x00FF;
    return 1;
}

// Push accumulator on stack.
uint8_t SY6502::PHA(){
    write(0x0100 + stkp, a);
    stkp--;
    return 0;
}
// Get stack value and push to accumulator.
uint8_t SY6502::PLA(){
    stkp++;
    a = read(0x0100 + stkp);
    set_flag(Z, a == 0x00);
    set_flag(N, a & 0x80);
    return 0;
}

// Return from interrupt.
uint8_t SY6502::RTI(){
    stkp++;
    status = read(0x0100 + stkp);
    status &= ~B;
    status &= ~U;

    stkp++;
    pc = (uint16_t)read(0x0100 + stkp);
    stkp++;
    pc |= (uint16_t)read(0x0100 + stkp) << 8;
    return 0;
}

// Uncovered operations in the video.
// Arithmetic shift left.
uint8_t SY6502::ASL(){
    fetch();
    auto temp = (uint16_t)fetched << 1;
    set_flag(C, (temp & 0xFF00) > 0);
    set_flag(Z, (temp & 0x00FF) == 0x00);
    set_flag(N, temp & 0x80);

    if (instruction_table[opcode].addrmode == &SY6502::IMP)
        a = temp & 0x00FF;
    else
        write(addr_abs, temp & 0x00FF);
    return 0;
}
// Test bit in memory with accumulator.
uint8_t SY6502::BIT(){
    fetch();
    auto temp = a & fetched;
    set_flag(Z, (temp & 0x00FF) == 0x00);
    set_flag(N, fetched & (1 << 7));
    set_flag(V, fetched & (1 << 6));
    return 0;
}
// Force break.
uint8_t SY6502::BRK(){
    pc++;

    set_flag(I, 1);
    write(0x0100 + stkp, (pc >> 8) & 0x00FF);
    stkp--;
    write(0x0100 + stkp, pc & 0x00FF);
    stkp--;

    set_flag(B, 1);
    write(0x0100 + stkp, status);
    stkp--;
    set_flag(B, 0);

    pc = (uint16_t)read(0xFFFE) | ((uint16_t)read(0xFFFF) << 8);
    return 0;
}
// Compare memory and accumulator.
uint8_t SY6502::CMP(){
    fetch();
    auto temp = (uint16_t)a - (uint16_t)fetched;
    set_flag(C, a >= fetched);
    set_flag(Z, (temp & 0x00FF) == 0x0000);
    set_flag(N, temp & 0x0080);
    return 1;
    
}
// Compare memory and index X.
uint8_t SY6502::CPX(){
    fetch();
    auto temp = (uint16_t)x - (uint16_t)fetched;
    set_flag(C, x >= fetched);
    set_flag(Z, (temp & 0x00FF) == 0x0000);
    set_flag(N, temp & 0x0080);
    return 0;
}
uint8_t SY6502::CPY(){
    fetch();
    auto temp = (uint16_t)y - (uint16_t)fetched;
    set_flag(C, y >= fetched);
    set_flag(Z, (temp & 0x00FF) == 0x0000);
    set_flag(N, temp & 0x0080);
    return 0;
}
// Decrement value in memory by one.
uint8_t SY6502::DEC(){
    fetch();
    auto temp = fetched - 1;
    write(addr_abs, temp & 0x00FF);
    set_flag(Z, (temp & 0x00FF) == 0x0000);
    set_flag(N, temp & 0x0000);
    return 0;
}
// Decrement index X by one.
uint8_t SY6502::DEX(){
    x--;
    set_flag(Z , x == 0x00);
    set_flag(N, x & 0x80);
    return 0;
}
uint8_t SY6502::DEY(){
    y--;
    set_flag(Z, y == 0x00);
    set_flag(N, y & 0x80);
    return 0;
}
// XOR memory with accumulator.
uint8_t SY6502::EOR(){
    fetch();
    a = a ^ fetched;
    set_flag(Z, a = 0x00);
    set_flag(N, a & 0x80);
    return 1;
}
// Increment memory/X/Y by one.
uint8_t SY6502::INC(){
    fetch();
    auto temp = fetched + 1;
    write(addr_abs, temp & 0x00FF);
    set_flag(Z, (temp & 0x00FF) == 0x0000);
    set_flag(N, temp & 0x0080);
    return 0;
}
uint8_t SY6502::INX(){
    x++;
    set_flag(Z , x == 0x00);
    set_flag(N, x & 0x80);
    return 0;
}
uint8_t SY6502::INY(){
    y++;
    set_flag(Z, y == 0x00);
    set_flag(N, y & 0x80);
    return 0;
}

// Jump to location.
uint8_t SY6502::JMP(){
    pc = addr_abs;
    return 0;
}
// Jump to sub-routine but also save location.
uint8_t SY6502::JSR(){
    pc--;

    write(0x0100 + stkp, (pc >> 8) & 0x00FF);
    stkp--;
    write(0x0100 + stkp, pc & 0x00FF);
    stkp--;

    pc = addr_abs;
    return 0;
}
// Load accumulator with memory.
uint8_t SY6502::LDA(){
    fetch();
    a = fetched;
    set_flag(Z, a == 0x00);
    set_flag(N, a & 0x80);
    return 1;
}
uint8_t SY6502::LDX(){
    fetch();
    x = fetched;
    set_flag(Z, x == 0x00);
    set_flag(N, x & 0x80);
    return 1;
}
uint8_t SY6502::LDY(){
    fetch();
    y = fetched;
    set_flag(Z, y == 0x00);
    set_flag(N, y & 0x80);
    return 1;
}

// Shift one bit right.
uint8_t SY6502::LSR(){
    fetch();
    set_flag(C, fetched & 0x0001);
    auto temp = fetched >> 1;
    set_flag(Z, (temp & 0x00FF) == 0x0000);
    set_flag(N, temp & 0x0080);

    if (instruction_table[opcode].addrmode == &SY6502::IMP)
        a = temp & 0x00FF;
    else
        write(addr_abs, temp & 0x00FF);

    return 0;
}
// No operation.
uint8_t SY6502::NOP(){
    switch (opcode){
        case 0x1C:
        case 0x3C:
        case 0x5C:
        case 0x7C:
        case 0xDC:
        case 0xFC:
            return 1;
            break;
    }
    return 0;
}
// "OR" memory with accumulator.
uint8_t SY6502::ORA(){
    fetch();
    a = a | fetched;
    set_flag(Z, a = 0x00);
    set_flag(N, a & 0x80);
    return 1;
}
// Push status register to stack.
uint8_t SY6502::PHP(){
    write(0x0100 + stkp, status | B | U);
    set_flag(B, 0);
    set_flag(U, 0);
    stkp--;
    return 0;
}
// Pop status register to accumulator.
uint8_t SY6502::PLP(){
    stkp++;
    status = read(0x0100 + stkp);
    set_flag(U, 1);
    return 0;
}
// Rotate one bit left/right.
uint8_t SY6502::ROL(){
    fetch();
    auto temp = (uint16_t)(fetched << 1) | get_flag(C);
    set_flag(C, fetched & 0x01);
    set_flag(Z, (temp & 0x00FF) == 0x00);
    set_flag(N, temp & 0x0080);
    if (instruction_table[opcode].addrmode == &SY6502::IMP)
        a = temp & 0x00FF;
    else
        write(addr_abs, temp & 0x00FF);
    return 0;
}
uint8_t SY6502::ROR(){
    fetch();
    auto temp = (uint16_t)(get_flag(C) << 7) | (fetched >> 1);
    set_flag(C, fetched & 0x01);
    set_flag(Z, (temp & 0x00FF) == 0x00);
    set_flag(N, temp & 0x0080);
    if (instruction_table[opcode].addrmode == &SY6502::IMP)
        a = temp & 0x00FF;
    else
        write(addr_abs, temp & 0x00FF);
    return 0;
}

// Return from subroutine.
uint8_t SY6502::RTS(){
    stkp++;
    pc = (uint16_t)read(0x0100 + stkp);
    stkp++;
    pc |= (uint16_t)read(0x0100 + stkp) << 8;
    pc++;
    return 0;
}
// Set flags.
uint8_t SY6502::SEC(){
    set_flag(C, true);
    return 0;
}
uint8_t SY6502::SED(){
    set_flag(D, true);
    return 0;
}
uint8_t SY6502::SEI(){
    set_flag(I, true);
    return 0;
}
// Store Accumulator/X/Y to address.
uint8_t SY6502::STA(){
    write(addr_abs, a);
    return 0;
}
uint8_t SY6502::STX(){
    write(addr_abs, x);
    return 0;
}
uint8_t SY6502::STY(){
    write(addr_abs, y);
    return 0;
}
// Transfer accumulator to X/Y register.
uint8_t SY6502::TAX(){
    x = a;
    set_flag(Z, x == 0x00);
    set_flag(N, x & 0x80);
    return 0;
}
uint8_t SY6502::TAY(){
    y = a;
    set_flag(Z, y == 0x00);
    set_flag(N, y & 0x80);
    return 0;
}
// Transfer stack pointer to X register.
uint8_t SY6502::TSX(){
    x = stkp;
    set_flag(Z, x == 0x00);
    set_flag(N, x & 0x80);
    return 0;
}
// Transfer X to accumulator.
uint8_t SY6502::TXA(){
    a = x;
    set_flag(Z, a = 0x00);
    set_flag(N, a & 0x80);
    return 0;
}
// Transfer X to stack resister.
uint8_t SY6502::TXS(){
    stkp = x;
    return 0;
}
// Transfer Y to accumulator.
uint8_t SY6502::TYA(){
    a = y;
    set_flag(Z, a = 0x00);
    set_flag(N, a & 0x80);
    return 0;
}

// Illegal opcode placeholder.
uint8_t SY6502::XXX(){
    return 0;
}