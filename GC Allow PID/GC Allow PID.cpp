#include <Windows.h>
#include <iostream>

#define GIVE_ALL_RIGHTS_IOCTL 0x2016E040

#pragma pack(push, 1)
struct _GC_INFO
{
	bool  AddMultipleProcesses = false;
	DWORD GCLauncher_pid = 0;
	DWORD SteamPid = 0;
	DWORD pid_to_allow = 0;
	DWORD pids[4095];
	DWORD NewCount = 0;
} GC_INFO;

#pragma pack(pop)

bool SendGCIOCTL() {
	HANDLE hDevice = INVALID_HANDLE_VALUE; //
	BOOL bResult = FALSE;
	DWORD junk = 0;

	hDevice = CreateFile(LR"(\\.\EMAC Secure)",
		FILE_ALL_ACCESS,
		0,
		nullptr,
		OPEN_EXISTING,
		0,
		nullptr);

	if (hDevice == INVALID_HANDLE_VALUE)
		return false;

	bResult = DeviceIoControl(hDevice,
		GIVE_ALL_RIGHTS_IOCTL,
		(void*)&GC_INFO, sizeof(_GC_INFO),
		NULL, NULL,              
		&junk,                   
		(LPOVERLAPPED)NULL);     

	CloseHandle(hDevice);

	return bResult;
}

int main(int argc, char * argv[])
{
	if (argc < 2) {
		std::cout << "Wrong Usage, please enter the PID to receive the access" << std::endl;
		Sleep(1000);
		return -1;
	}

	GC_INFO.pid_to_allow = atoi(argv[1]);

	if (SendGCIOCTL()) {
		std::cout << "Success" << std::endl;
		std::cout << "Now the pid [ " << std::hex << GC_INFO.pid_to_allow << " ] can open handles to csgo" << std::endl;
		Sleep(1000);
		return 1;
	}
	else {
		std::cout << "The request failed, are you sure you entered a valid PID?" << std::endl;
		Sleep(1000);
		return -1;
	}
	return 1;
}