#include "RAM.h"

bool RAM::_Write(uint16_t address, byte value)
{
    if (address >= memSize)
    {
        LOGP("Trying to write outside of the allocated RAM size", "RAM::_Write");
        throw exception("OutOfBoundException");
        return false;
    }

#if LOG_MEM == true
    LOGP("Writing to 0x" << hex << (int)value << " to 0x" << hex << address, "RAM::_Write");
#endif

    memory[address] = value;

    return true;
}

byte& RAM::_Read(uint16_t address)
{
    // TODO: tu wstawiæ instrukcjê return
    if (address >= memSize)
    {
        LOGP("Trying to read outside of the allocated RAM size", "RAM::_Read");
        throw exception("OutOfBoundException");
        return MemoryModule::nullValue;
    }

#if LOG_MEM == true
    byte val = memory[address];
    LOGP("Reading from 0x" << hex << address << ": 0x" << hex << (int)val, "RAM::_Read");

    return val;
#else
    return memory[address];
#endif
}
