#pragma once
typedef HRESULT(APIENTRY* tPresent)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
HRESULT APIENTRY hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);

class init {
private:
	static init* inst;
	void* pTable[1] = { 0 };
	init() {};
public:
	bool initi = false;
	tPresent oPresent = nullptr;

	init(init&) = delete;
	init& operator =(const init&) = delete;

	
	uintptr_t* Detour(uintptr_t* src, BYTE* dst, unsigned int len);
	uintptr_t* Detour2(uintptr_t* src, BYTE* dst, unsigned int len);
	static init* getinst() {
		if (inst == nullptr) inst = new init();
		return inst;
	}
	bool enable();
};