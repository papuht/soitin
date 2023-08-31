#pragma once

#define ARDUINO_WAIT_TIME 2000
#define INPUT_DATA_BYTES 7

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

class SerialPort {
private:
	HANDLE handleToCOM;
	bool connected;
	COMSTAT status;
	DWORD errors;

public:
	SerialPort(char* portName);
	~SerialPort();

	int ReadSerialPort(char* buffer, unsigned int buf_size);
	bool isConnected();
};
