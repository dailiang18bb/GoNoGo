#include "ArduinoBoard.h"

ArduinoBoard::ArduinoBoard(const char portname[5])//example: "COM3", and portname[4]='\0' 
{//char portname[5]="COM3"
	this->lpOutBuffer = &this->out;
	this->dwBytesWrite = 1;
	/*
	wchar_t portN[5];
	mbstowcs(portN, portname, 5);
	LPCWSTR SerialPortName = portN;//Serial port name
	*/
	hCom = CreateFile(portname, //Serial port name
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

void ArduinoBoard::SendTheResult(char PorF, bool CorW)
{
	/*
	out:
		F->button off
		PC->light off button on //big P
		PW->light on button off //small p
	*/
	if (PorF == 'F') {
		this->out = 'F';
		SendToArduino();
	}
	else if (PorF == 'P') {
		if (CorW) {
			this->out = 'P';//big
			SendToArduino();
		}
		else
		{
			this->out = 'p';//small
			SendToArduino();
		}
	}

}

void ArduinoBoard::CameraTrigger(bool Sw)//true=camera on. false=off //control the relay for 
{
	if (Sw) {
		this->out = 'T';//trigger
		SendToArduino();
	}
	else {
		this->out = 'S';//stop
		SendToArduino();
	}
}

void ArduinoBoard::ArduinoStop()
{
	CloseHandle(hCom);
}

void ArduinoBoard::SendToArduino()
{
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	ClearCommError(hCom, &dwErrorFlags, &ComStat);
	WriteFile(hCom, lpOutBuffer, dwBytesWrite, &dwBytesWrite, NULL);
	//writefile(�����д�����ݣ�д���ֽ�����ʵ��д���ֽ�����ָ�룩��OVERLAPPED �ṹ: һ���趨Ϊ NULL)
	Sleep(50);
}

