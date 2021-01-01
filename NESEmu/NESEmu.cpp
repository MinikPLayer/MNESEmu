#include <iostream>
#include "NES.h"


int main()
{
    std::cout << "Hello World!\n";

    InitLog();
    NES n;
    cout << "Loading ROM result: " << n.LoadROM("test.nes") << endl;
    try
    {
        n.Run(0xC000); // because of test.nes automation utility
        //n.Run();
    }
    catch (exception e)
    {
        n.cpu.PrintUnimplemented();
        uint16_t status1 = n.memory.Read(0x02);
        uint16_t status2 = n.memory.Read(0x03);

        LOGP("Status: 0x" << hex << status1 << " / 0x" << hex << status2, "NESEmu");
        CloseLog();
        throw e;
    }

    uint16_t s1 = n.memory.Read(0x02);
    uint16_t s2 = n.memory.Read(0x03);

    LOGP("Status: 0x" << hex << s1 << " / 0x" << hex << s2, "NESEmu");
    CloseLog();
    return 0;
}
