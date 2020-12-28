#include "Memory.h"

MemoryModule::MemoryModule(string name, bool writeable, bool readable, uint16_t minAddr, uint16_t maxAddr)
{
	this->name = name;
	this->writeable = writeable;
	this->readable = readable;
	this->minAddr = minAddr;
	this->maxAddr = maxAddr;
}

byte MemoryModule::nullValue = 0;