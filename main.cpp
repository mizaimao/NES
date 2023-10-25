#include "viewer.cpp"
#include "bus6502.h"
// #include "cpu.h"
 
int main()
{
    Bus bus;
    std::cout << "super chicken current cpu cycle: " << bus.cpu.cycles << endl;
    // std::cout << "circular cpu cycle: " << bus.cpu.bus->cpu.cycles << endl;

    Viewer Viewer;
    Viewer.launch_window();
    std::cout<< "Viewer Launched" <<std::endl;

    //SY6502 cpu;
 
    return 0;
}
