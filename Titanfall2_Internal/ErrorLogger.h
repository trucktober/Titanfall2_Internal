#pragma once
#include <Windows.h>
#include <fstream>
#include <iostream>

class ErrorLogger
{
private:
	FILE* errorConsole;
	std::wofstream logFile;
	static ErrorLogger* instance;
public:
	ErrorLogger();
	~ErrorLogger();
	ErrorLogger(ErrorLogger&) = delete;
	ErrorLogger& operator=(const ErrorLogger&) = delete;
	static ErrorLogger* getinst() {
		if (instance == nullptr) instance = new ErrorLogger;
		return instance;
	}
	static ErrorLogger* delinst() {
		delete instance;
		return instance = nullptr;
	}

	template <typename T>
	void Print(T errMsg) {
		std::cout << std::hex << "/0x" << errMsg << std::endl;
	}

	template <typename T>
	void PrintArr(T errMsgArr[], int arrSize) {
		for (int i = 0; i < arrSize; ++i) {
			std::cout << std::hex << "/0x" << static_cast<int>(errMsgArr[i]);
		}
		std::cout << std::endl;
	}

	void PrintHR(HRESULT hr);
	void PrintError(const wchar_t* errMsg);
	void Error(HRESULT hr);
	void Error(const wchar_t* errMsg);
	void LogError(HRESULT hr);
	void LogError(const wchar_t* errMsg);
	void PopupError(HRESULT hr);
	void PopupError(const wchar_t* errMsg);
	void log(HRESULT hr, wchar_t* log);
};

