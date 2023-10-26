#include "viewer.cpp"
#include "bus6502.cpp"
#include "cpu.cpp"
#include <sstream> 
using namespace::std;
 
int main()
{
    Bus bus;
    uint16_t addr = 0x0012;

    cout << "super chicken current x register: " << bus.cpu.x << endl;
    
    std::stringstream ss;
    ss << "A2 0A 8E 00 00 A2 03 8E 01 00 AC 00 00 A9 00 18 6D 01 00 88 D0 FA 8D 02 00 EA EA EA";
    uint16_t nOffset = 0x0180;
    bus.cpu.pc = nOffset;
    while (!ss.eof())
    {
        std::string b;
        ss >> b;
        bus.cpu_ram[nOffset++] = (uint8_t)std::stoul(b, nullptr, 16);
    }
    // Set Reset Vector
    bus.cpu_ram[0xFFFC] = 0x00;
    bus.cpu_ram[0xFFFD] = 0x80;
    // cout<< "CPU flags  N:" << to_string(bus.cpu.status & SY6502::N) << "  V:" << to_string(bus.cpu.status & SY6502::V) << endl;
    // cout<< "CPU flags  U:" << to_string(bus.cpu.status & SY6502::U) << "  B:" << to_string(bus.cpu.status & SY6502::B) << endl;
    // cout<< "CPU flags  D:" << to_string(bus.cpu.status & SY6502::D) << "  I:" << to_string(bus.cpu.status & SY6502::I) << endl;
    // cout<< "CPU flags  Z:" << to_string(bus.cpu.status & SY6502::Z) << "  C:" << to_string(bus.cpu.status & SY6502::C) << endl;


    int char_per_line = 16;
    for (int i = 0x0000; i < 0x00FF; i+=char_per_line){
        for (int j = 0; j < char_per_line; j++){
            cout << to_string( bus.cpu_ram[i + j] ) << " ";
        }
        cout << endl;
    }

    Viewer Viewer(&bus);
    Viewer.launch_window();
    cout<< "Viewer Launched" <<endl;

 
    return 0;
}
