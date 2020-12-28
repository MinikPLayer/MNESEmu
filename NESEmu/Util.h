#pragma once

#include "Defines.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#define IsBetween(x, low, high) (x >= low && x <= high)


#define LOGN(data) cout << data;

#define LOGE(data, prefix, nl) LOGN("[" << prefix << "] " << data << nl);
#define LOGP(data, prefix) LOGE(data, prefix, "\n");
#define LOG(data) LOGN(data << endl);

byte LoadNext(ifstream& stream, string part = "" /* for example: header, prg, chr, etc */);

// Get bit from p position of the value number
bool GetBit(byte value, int p);

// Get k bits from p position
byte GetBits(byte value, int p, int k);

void SetBit(byte& number, int p, bool value);