#include "SerialPort.h"

SerialPort::SerialPort(char* portName) {
	
	errors = 0;
	status = { 0 };
	connected = false;

	//create & open teh COM input device
	handleToCOM = CreateFileA(static_cast<LPCSTR>(portName), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);


	DWORD errorMsg = GetLastError();

	if (errorMsg == 2) {
		printf("No sensor connected \n");
	}
	else if (errorMsg == 5) {
		printf("Another application is already using the sensor");
	}

	else if (errorMsg == 0) {

		DCB dcbSerialParameters = { 0 };

		if (!GetCommState(handleToCOM, &dcbSerialParameters)) {
			printf("faile to get current serial parameters");
		}
		else {
			dcbSerialParameters.BaudRate = CBR_115200;
			dcbSerialParameters.ByteSize = 8;
			dcbSerialParameters.StopBits = ONESTOPBIT;
			dcbSerialParameters.Parity = NOPARITY;
			dcbSerialParameters.fDtrControl = DTR_CONTROL_ENABLE;

			if (!SetCommState(handleToCOM, &dcbSerialParameters)) {
				printf("ALERT: could not set Serial port parameters \n");
			}
			else {
				connected = true;
				PurgeComm(handleToCOM, PURGE_RXCLEAR | PURGE_TXCLEAR);
				Sleep(ARDUINO_WAIT_TIME);
			}

		}


	}

}

SerialPort::~SerialPort() {
	if (connected == true) {
		connected = false;
		CloseHandle(handleToCOM);
	}

}

int SerialPort::ReadSerialPort(char* buffer, unsigned int buf_size) {
	DWORD bytesRead;
	unsigned int toRead = 0;

	ClearCommError(handleToCOM, &errors, &status);

	if (status.cbInQue > 0) {
		if (status.cbInQue > buf_size) {
			toRead = buf_size;
		}
		else toRead = status.cbInQue;
	}

	if (ReadFile(handleToCOM, buffer, toRead, &bytesRead, NULL)) {
		return bytesRead;
	}
	return 0;

}

bool SerialPort::isConnected() {
	return connected;
}