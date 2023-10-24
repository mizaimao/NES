#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

//#include "src/include/SDL2/sdl.h"
#include <SDL2/sdl.h>
#include "viewer.cpp"

using namespace std;

int main() {

    VIEWER::Viewer viewer;
    cout << viewer.width << " " << viewer.height << endl;
    viewer.start_viewer();

    

    return 0;
}
