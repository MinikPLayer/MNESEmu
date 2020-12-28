#include <iostream>
#include "NES.h"


int main()
{
    std::cout << "Hello World!\n";

    NES n;
    cout << "Loading ROM result: " << n.LoadROM("test.nes") << endl;
    n.Run(0xC000); // because of test.nes automation utility

    return 0;
}
