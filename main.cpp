#include "viewer.cpp"
#include "bus6502.h"
// #include "cpu.h"
 
int main()
{
    Viewer Viewer;
    Viewer.launch_window();
    std::cout<< "Viewer Launched" <<std::endl;
    
    Bus bus;
    //SY6502 cpu;
 
    return 0;
}
