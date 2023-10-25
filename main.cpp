#include "viewer.cpp"
#include "bus6502.h"
// #include "cpu.h"
using namespace::std;
 
int main()
{
    Bus bus;
    cout << "super chicken current x register: " << bus.cpu.x << endl;
    bus.cpu.clock();

    int char_per_line = 16;
    for (int i = 0x0000; i < 0x00FF; i+=char_per_line){
        for (int j = 0; j < char_per_line; j++){
            cout << bus.cpu_ram[i + j] << " ";
        }
        cout << endl;
    }

    Viewer Viewer;
    Viewer.launch_window();
    cout<< "Viewer Launched" <<endl;

 
    return 0;
}
