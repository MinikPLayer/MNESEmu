#include <iostream>
#include "NES.h"


int main()
{
    std::cout << "Hello World!\n";

    NES n;
    cout << "Loading ROM result: " << n.LoadROM("hello.nes") << endl;
    n.Run();

    return 0;
}
