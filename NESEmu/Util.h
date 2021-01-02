#pragma once

#include "Defines.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

extern ofstream logStream;

#define IsBetween(x, low, high) (x >= low && x <= high)

#define FILE_LOG false
#define CONSOLE_LOG true

#if FILE_LOG
	#if CONSOLE_LOG
		#define LOGN(data) {cout << data; logStream << data; }
	#else
		#define LOGN(data) { logStream << data; }
	#endif
#else
#if CONSOLE_LOG
		#define LOGN(data) {cout << data;}
	#else
		#define LOGN(data) {}
	#endif
#endif

#define LOGE(data, prefix, nl) LOGN("[" << prefix << "] " << data << nl);
#define LOGP(data, prefix) LOGE(data, prefix, "\n");
#define LOG(data) LOGN(data << endl);

void InitLog(string filePath = "log.txt");
void CloseLog();

byte LoadNext(ifstream& stream, string part = "" /* for example: header, prg, chr, etc */);

// Get bit from p position of the value number
bool GetBit(byte value, int p);

// Get k bits from p position
#define GetBits(value, p, k) ((1 << k) - 1) & (value >> p)
//byte GetBits(byte value, int p, int k);

void SetBit(byte& number, int p, bool value);