#include "ErrorLogger.h"
ErrorLogger::ErrorLogger() : errorConsole(nullptr) {
	AllocConsole();
	freopen_s(&errorConsole, "CONOUT$", "w", stdout);
	logFile.open("log.txt", std::ios::app);
	if (!logFile.is_open()) PrintError(L"Logger Failed :/");
}

ErrorLogger::~ErrorLogger() {
	std::cout << "Deleting System_32 ";
	for (int i = 0; i < 10; i++) { std::cout << " ."; Sleep(50); }
	FreeConsole();
	if (logFile.is_open()) logFile.close();
}


void ErrorLogger::PrintHR(HRESULT hr) {
	WCHAR buffer[256];
	swprintf(buffer, sizeof(buffer) / sizeof(WCHAR), L"Error Code: 0x%08X", hr);
	std::cout << "Error Code: 0x%08X" << hr << std::endl;
}

void ErrorLogger::PrintError(const wchar_t* errMsg) {
	std::wcout << errMsg << std::endl;
}

void ErrorLogger::LogError(HRESULT hr) {
	WCHAR buffer[256];
	swprintf(buffer, sizeof(buffer) / sizeof(WCHAR), L"Error Code: 0x%08X", hr);
	logFile << buffer << " Error!" << std::endl;
}

void ErrorLogger::LogError(const wchar_t* errMsg) {
	logFile << errMsg << " Error!" << std::endl;
}

void ErrorLogger::Error(HRESULT hr) {
	WCHAR buffer[256];
	swprintf(buffer, sizeof(buffer) / sizeof(WCHAR), L"Error Code: 0x%08X", hr);
	std::cout << "Error Code: 0x%08X" << hr << std::endl;
	logFile << buffer << " Error!" << std::endl;
	MessageBox(NULL, buffer, L" fucking loser!!", MB_ICONERROR | MB_OK);
}

void ErrorLogger::PopupError(const wchar_t* errMsg) {
	MessageBox(NULL, errMsg, NULL, MB_ICONERROR | MB_OK);
}

void ErrorLogger::PopupError(HRESULT hr) {
	WCHAR buffer[256];
	swprintf(buffer, sizeof(buffer) / sizeof(WCHAR), L"Error Code: 0x%08X", hr);
	MessageBox(NULL, buffer, L" fucking loser!!", MB_ICONERROR | MB_OK);
}

void ErrorLogger::Error(const wchar_t* errMsg) {
	std::wcout << errMsg << std::endl;
	logFile << errMsg << " Error!" << std::endl;
	MessageBox(NULL, errMsg, NULL, MB_ICONERROR | MB_OK);
}

void ErrorLogger::log(HRESULT hr, wchar_t* log) {
	WCHAR buffer[256];
	swprintf(buffer, sizeof(buffer) / sizeof(WCHAR), L"0x%08X");
	wcscat_s(log, sizeof(buffer) / sizeof(WCHAR), buffer);
	logFile << log << " Success!" << std::endl;
	std::wcout << log << " Success!" << std::endl;
	
}

