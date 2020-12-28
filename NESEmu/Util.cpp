#include "Util.h"

byte LoadNext(ifstream& stream, string part)
{
	byte b;
	if (!(stream >> b))
	{
		string d = "Error reading rom data";
		if (part.size() > 0)
		{
			d += " in " + part + " section";
		}
		throw exception(d.c_str());
		return false;
	}

	return b;
}

bool GetBit(byte value, int p)
{
	return (value >> p) & 1;
}

/*byte GetBits(byte value, int p, int k)
{
	return ((1 << k) - 1) & (value >> p);
}*/

void SetBit(byte& number, int p, bool value)
{
	if (value)
		number |= 1UL << p;
	else
		number &= ~(1UL << p);
}
