#pragma once
#include <Windows.h>
#include <iostream>

class ArduinoBoard
{
public:
	ArduinoBoard(char portname[5]);
	void SendTheResult(char PorF, bool CorW);
private:
	HANDLE hCom;
	DWORD dwBytesWrite;
	char out;
	char* lpOutBuffer;
	void SendToArduino();
};
