#include "ROM.h"
#include "Util.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <iterator>
#include <vector>

#include "Mappers.h"

bool ROM::Header::Load(ifstream& stream)
{
	const char NES_HEADER[4] = "NES";
	for (int i = 0; i < 3; i++)
	{
		char c = '\0';
		c = LoadNext(stream, "header");

		if (c != NES_HEADER[i])
		{
			cout << "Not an iNES header" << endl;
			return false;
		}
	}
	byte temp = '\0';
	stream >> temp; // EOL

	temp = LoadNext(stream, "header");
	cout << "PRG ROM size: " << ((int)temp * 16) << "KB" << endl;
	prgSize = temp;

	temp = LoadNext(stream, "header");
	cout << "CHR ROM size: " << ((int)temp * 16) << "KB" << endl;
	chrSize = temp; 

	// Flags
	for (int i = 0; i < 5; i++)
	{
		flags[i] = LoadNext(stream, "header");
		cout << "Flags[" << i << "]: " << (int)flags[i] << endl;
	}

	mirroring = GetBit(flags[0], 0);
	prgRam = GetBit(flags[0], 1);
	trainer = GetBit(flags[0], 2);
	fourScreenVRAM = GetBit(flags[0], 3);

	byte lowerMapper = GetBits(flags[0], 4, 4);
	byte higherMapper = GetBits(flags[1], 0, 4);

	mapper = (higherMapper << 4) + lowerMapper;
	cout << "Mapper: " << (int)mapper << endl;

	// Padding
	for (int i = 0; i < 5; i++)
	{
		LoadNext(stream, "header");
	}
	return true;
}



bool ROM::Load(string path)
{
	try
	{
		return _Load(path);
	}
	catch (exception e)
	{
		cout << "Error loading rom: \n" << e.what() << endl;
		return false;
	}
}

ROM::~ROM()
{
	if (mapper != nullptr)
	{
		delete mapper;
		mapper = nullptr;
	}
}

bool ROM::_Load(string path)
{
	ifstream stream(path, ios_base::binary);
	if (!stream.good())
	{
		cout << "Error opnenig rom file: " << path << endl;
		return false;
	}

	if (!header.Load(stream))
	{
		return false;
	}

	mapper = GetMapper(header.mapper);
	if (mapper == nullptr)
		return false; // mapper not found

	int prgSize = 16384 * header.prgSize;
	//mapper->prg.reserve(prgSize);
	char temp = '\0';
	// Load program data
	/*for (int i = 0; i < prgSize; i++)
	{
		temp = LoadNext(stream, "PRG");
		mapper->prg.push_back(temp);
	}*/
	//mapper->prg 
	mapper->prg = vector<byte>(prgSize, 0);
	stream.read(reinterpret_cast<char*>(&(mapper->prg[0])), prgSize);
	stream.close();

	return true;
}

byte& ROM::_Read(uint16_t address)
{
	return mapper->Read(address);
}
