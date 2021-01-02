#include "PatternTable.h"

bool PatternTable::_Write(uint16_t address, byte value)
{
    if (address > 0x1000)
    {
        LOGP("Adress 0x" << hex << address << " is outside of memory range", "PatternTable::_Write");
        throw("OutOfRangeException");
        return 0;
    }

    memory[address] = value;
}

byte PatternTable::_Read(uint16_t address)
{
    if (address > 0x1000)
    {
        LOGP("Adress 0x" << hex << address << " is outside of memory range", "PatternTable::_Read");
        throw("OutOfRangeException");
        return 0;
    }

    return memory[address];
}
