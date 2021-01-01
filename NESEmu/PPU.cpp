#include "PPU.h"

bool PPU::PPUReg::_Write(uint16_t address, byte value)
{
    LOGP("PPU IS NOT IMPLEMENTED YET", "PPUReg::_Write");
    return false;
}

byte& PPU::PPUReg::_Read(uint16_t address)
{
    LOGP("PPU IS NOT IMPLEMENTED YET", "PPUReg::_Write");
    return MemoryModule::nullValue;
}

PPU::PPU()
{
    Init();
}

void PPU::Init()
{

}

void PPU::Reset()
{
    reg.Write(0, 0);
}
