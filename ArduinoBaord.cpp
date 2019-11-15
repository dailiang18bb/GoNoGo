#include "ArduinoBaord.h"

ArduinoBaord::ArduinoBaord(char portname[5])//example: "COM3", and portname[4]='\0' 
{
	lpOutBuffer = &out;
	dwBytesWrite = 1;
	////
	wchar_t portN[5];
	mbstowcs(portN, portname, 5);
	LPCWSTR SerialPortName = portN;//Serial port name
	hCom = CreateFile(SerialPortName, //Serial port name
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL
	);
	if (hCom == (HANDLE)-1)
	{
		std::cout << "Serial port connection failed";
	}
	else {
		SetupComm(hCom, 64, 64);
		COMMTIMEOUTS TimeOuts;
		TimeOuts.ReadIntervalTimeout = 2;
		TimeOuts.ReadTotalTimeoutMultiplier = 50;
		TimeOuts.ReadTotalTimeoutConstant = 100;
		TimeOuts.WriteTotalTimeoutMultiplier = 500;
		TimeOuts.WriteTotalTimeoutConstant = 1000;
		SetCommTimeouts(hCom, &TimeOuts);

		DCB dcb;
		GetCommState(hCom, &dcb);
		dcb.BaudRate = CBR_9600;
		dcb.ByteSize = 8;
		dcb.Parity = NOPARITY;
		dcb.StopBits = ONESTOPBIT;
		dcb.fParity = FALSE;
		SetCommState(hCom, &dcb);

		PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR);
		Sleep(1000);//wait at least900ms to wait MCU to ready
	}
}

void ArduinoBaord::SendTheResult(char R)
{
	if ( (R == 'p') || (R == 'f') ) {
		this->out = R;
		SendToArduino();
	}
}

void ArduinoBaord::SendToArduino()
{
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	ClearCommError(hCom, &dwErrorFlags, &ComStat);
	WriteFile(hCom, lpOutBuffer, dwBytesWrite, &dwBytesWrite, NULL);
	//writefile(句柄，写入内容，写入字节数，实际写入字节数（指针），OVERLAPPED 结构: 一般设定为 NULL)
	Sleep(50);
}

