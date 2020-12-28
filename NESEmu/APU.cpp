#include "APU.h"

bool APU::APUReg::_Write(uint16_t address, byte value)
{
    LOGP("APU IS NOT IMPLEMENTED YET", "APUReg::_Write");
    return false;
}

byte& APU::APUReg::_Read(uint16_t address)
{
    LOGP("APU IS NOT IMPLEMENTED YET", "APUReg::_Write");
    return MemoryModule::nullValue;
}
