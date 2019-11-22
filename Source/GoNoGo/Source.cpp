#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <iostream>
#include <string>
#include <windows.h>
#include "ArduinoBoard.h"

#pragma comment(lib, "ws2_32.lib")
using namespace std;

#define DEFAULT_PORT 2005
#define DETAULT_ADDRESS "169.254.92.130"
#define DEFAULT_BUF_LEN 30
#define DEFAULT_PART_NUMBER_LEN 23
#define SOFTWARE_NAME "BAR CODE INTERFACE APPLICATION"
#define SOFTWARE_VERSION "V1.0"

// Input Singal String length 29 ">>P||DZF419042Z007112692968<<"
const char P2968[] = "DZF419042Z007112692968";
//const char P2972[] = "DZF419042Z007112692972";
const char P2972[] = "DZF419198Z032712692972";

bool cameraTrigger = false;


// Show welcomeScreen
void welcomeScreen() {
	cout << "////////////////////////////////////////////////////////" << endl;
	cout << SOFTWARE_NAME << " " << SOFTWARE_VERSION << endl;
	cout << "Application initiating." << endl;
}


// Initialize WinSocket
void initialization() {
	int err;
	WORD versionRequired;
	WSADATA wsadata;
	versionRequired = MAKEWORD(2, 2);
	err = WSAStartup(versionRequired, &wsadata);

	if (!err) {
		cout << "Initialize WinSocket Success." << endl;
	}
	else {
		cout << "Initialize WinSocket Fail!" << endl;
		WSACleanup();
	}

	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2) {
		cout << "Socket Library version not Match!" << endl;
		WSACleanup();
	}
	else {
		cout << "Socket Library version correct." << endl;
	}
}



// Compare two parts code
bool barCodeCompare(const char* selected, char* scanned) {
	if (strcmp(selected, scanned) == 0) {
		cout << "Part check correct." << endl;
		return true;
	}
	else {
		cout << "!!!!Wrong Part!!!! NG!!!!" << endl;
		return false;
	}
}


// MAIN
int main() {
	welcomeScreen();
	ArduinoBoard board1("COM3");

	/*
	// Test
	char test1[] = ">>P||DZF419042Z007112692968<<";
	//char sss[23];
	char sss[30];
	//strncpy(sss, test1, );
	strncpy(sss, test1, sizeof(test1));
	*/


	// Declare variables
	char recv_buf[DEFAULT_BUF_LEN];
	int recv_len = 0;
	int recv_buf_len = DEFAULT_BUF_LEN;
	int testing_count = 0;
	char scanned_part_no[DEFAULT_PART_NUMBER_LEN];
	char selected_part_no[DEFAULT_PART_NUMBER_LEN];
	int user_input_part_no;
	bool part_input_state = FALSE;


	// Configure Socket
	initialization();
	SOCKET clientSocket;
	SOCKADDR_IN server_addr;
	server_addr.sin_family = AF_INET;
	//server_addr.sin_addr.S_un.S_addr = inet_addr(DETAULT_ADDRESS);
	inet_pton(AF_INET, DETAULT_ADDRESS, &server_addr.sin_addr.s_addr);
	server_addr.sin_port = htons(DEFAULT_PORT);

	// Select parts
	do {
		cout << "=====================================================" << endl;
		cout << "Please type in the parts number you are work on(the last four digits):" << endl;
		cin >> user_input_part_no;
		switch (user_input_part_no)
		{
		case 2972:
			strcpy_s(selected_part_no, P2972);
			part_input_state = FALSE;
			break;
		case 2968:
			strcpy_s(selected_part_no, P2968);
			part_input_state = FALSE;
			break;
		default:
			part_input_state = TRUE;
			cout << "Wrong part number" << endl;
		}
	} while (part_input_state);

	cout << "Thank you! The part you're working on is " << user_input_part_no << "-" << selected_part_no << endl;
	cout << "=====================================================" << endl;




	// Create a socket
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (connect(clientSocket, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
		cout << "Server connection failed£¡" << endl;
		closesocket(clientSocket);
		WSACleanup();
		Sleep(2000);
		return 1;
	}
	else {
		cout << "Server connection success£¡" << endl;
	}
	/*
	// Telegram
	 do {
		testing_count++;
		recv_len = recv(clientSocket, recv_buf, recv_buf_len, 0);
		recv_buf[29] = '\0';
		if (recv_len < 0) {
			cout << "Receive message fail!" << endl;
			break;
		}
		else {
			cout << "Received: " << recv_buf << endl;
		}
	 } while (testing_count < 100);
	 
	// Test compare
	barCodeCompare(selected_part_no, scanned_part_no);
	*/	 


	int i,j=0;
	while (j<1000) {
		//no trigger, free run
		j++;
		short count = 0;
		do {
			recv_len = recv(clientSocket, recv_buf, recv_buf_len, 0);
			count++;
			if (count > 20) {
				break;
			}
		} while (recv_len <= 0);
		if (count > 20) {
			cout << "==============================================" << endl;
			cout << "Unable to get results from camera." << endl;
			cout << "Please check and restart the application." << endl;
			break;
		}
		recv_buf[29] = '\0';
    
		for (i = 0; i < (DEFAULT_PART_NUMBER_LEN-1) ; i++) {
			scanned_part_no[i] = recv_buf[5 + i];
		}
		scanned_part_no[i] = '\0';

		if (recv_buf[2] == 'P') {
			board1.SendTheResult(recv_buf[2], barCodeCompare(selected_part_no, scanned_part_no));
		}
		else if(recv_buf[2] == 'F') {
			board1.SendTheResult(recv_buf[2], false);
		}

		//board1.SendTheResult('p', true);
		//cout << recv_buf << endl;
		//cout << scanned_part_no << endl;
		//cout << selected_part_no << endl;
		//cout << strcmp(scanned_part_no, selected_part_no) << endl;


	}

	// Close and Cleanup
	board1.ArduinoStop();
	closesocket(clientSocket);
	WSACleanup();
	return 0;
}