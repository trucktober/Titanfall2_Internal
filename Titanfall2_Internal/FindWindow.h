#pragma once
#include "Includes.h"

class Wnd {
private:
	Wnd* inst;
	HWND hWnd;
public:
	Wnd(Wnd&) = delete;
	Wnd& operator=(const Wnd&) = delete;

	Wnd* getInst() {

	}

	struct WND_Data {
		unsigned long procid;
		HWND hWnd;
	};

	HWND hWnd_CW() {
		hWnd = CreateWindowEx(NULL, L"Fortnite", L"Battle Royale", NIS_HIDDEN, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
		return hWnd;
	}

	HWND hWnd_GW(unsigned long procid) {
		WND_Data wind;
		wind.procid = procid;
		wind.hWnd = 0;
		EnumWindows(hWnd_GCW, (LPARAM)&wind);
	}

	BOOL CALLBACK hWnd_GCW(HWND hWnd, LPARAM wind) {
		WND_Data hWind = *(WND_Data*)wind;
		unsigned long procid = 0;
		GetProcessIdOfThread(&procid);
		if (hWind.procid != procid || !hWnd_MW(hWnd)) return TRUE;
		hWind.hWnd = hWnd;
		return FALSE;
	}

	bool hWnd_MW(HWND hWnd) {
		return GetWindow(hWnd, GW_OWNER) == (HWND)0 && IsWindowVisible(hWnd);
	}
};

