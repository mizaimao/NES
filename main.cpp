#include "viewer.cpp"
#include "bus6502.cpp"
#include "cpu.cpp"
using namespace::std;
 
int main()
{
    Bus bus;
    uint16_t addr = 0x0012;

    cout << "super chicken current x register: " << bus.cpu.x << endl;
    bus.cpu.clock();
    
    cout<< "CPU flags  N:" << to_string(bus.cpu.status & SY6502::N) << "  V:" << to_string(bus.cpu.status & SY6502::V) << endl;
    cout<< "CPU flags  U:" << to_string(bus.cpu.status & SY6502::U) << "  B:" << to_string(bus.cpu.status & SY6502::B) << endl;
    cout<< "CPU flags  D:" << to_string(bus.cpu.status & SY6502::D) << "  I:" << to_string(bus.cpu.status & SY6502::I) << endl;
    cout<< "CPU flags  Z:" << to_string(bus.cpu.status & SY6502::Z) << "  C:" << to_string(bus.cpu.status & SY6502::C) << endl;

    bus.cpu.disassemble(0x0000, 0x0010);

    int char_per_line = 16;
    for (int i = 0x0000; i < 0x00FF; i+=char_per_line){
        for (int j = 0; j < char_per_line; j++){
            cout << bus.cpu_ram[i + j] << " ";
        }
        cout << endl;
    }

    Viewer Viewer(&bus);
    Viewer.launch_window();
    cout<< "Viewer Launched" <<endl;

 
    return 0;
}
