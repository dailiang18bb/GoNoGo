#pragma once
#include <Windows.h>
#include <iostream>

class ArduinoBoard
{
public:
	ArduinoBoard(const char portname[5]);
	void SendTheResult(char PorF, bool CorW);
	void CameraTrigger(bool Sw);
private:
	HANDLE hCom;
	DWORD dwBytesWrite;
	char out;
	char* lpOutBuffer;
	void SendToArduino();
};