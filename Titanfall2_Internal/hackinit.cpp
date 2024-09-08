#pragma once
#include "Includes.h"

Device* d = Device::getinst();
ErrorLogger* logger = ErrorLogger::getinst();

bool init::enable() {
    
	d->CreateDevice(pTable);
	oPresent = (tPresent)Detour((uintptr_t*)pTable[0], (BYTE*)hkPresent, 5);

    return true;
}

uintptr_t* init::Detour2(uintptr_t* src, BYTE* dst, unsigned int len) {
	uintptr_t* StlnByt, * pSrcLoc, * pGateway = nullptr;
	DWORD p;
	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &p);
	pSrcLoc = src - 0x2000;

	while (!pGateway) {
		pGateway = StlnByt = (uintptr_t*)VirtualAlloc(pSrcLoc, 1, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		pSrcLoc += 0x200;
	}

	BYTE hkBytes[] = {
		0xF2, 0x0F, 0x10, 0x04, 0x25, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, // movsd xmm0,  (double)hkFunction
		0xF2, 0x0F, 0x11, 0x04, 0x24,										  // movsd [rsp], xmm0
		0xDD, 0x04, 0x24,													  // fld qword ptr [rsp]
		0xDF, 0x3C, 0x24,													  // fistp qword ptr [rsp]
		0xC3																  // ret
	};

	BYTE hkBytes2[] = {
		0x48, 0xB8, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, // mov rax, hkFunction
		0xDD, 0x00,											// fld qword ptr [rax]
		0xDF, 0x3C, 0x24,									// fistp qword ptr [rsp]
		0xC3												// ret	
	};

	BYTE hkBytes3[] = {
		0x48, 0xB8, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, // movabs rax, hkFunction
		0xFF, 0xD0											// call rax
	};

	// if initial is a call instruction
	BYTE hkBytes4[] = {
		0x48, 0xC7, 0xC4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, // mov rsp, hkFunction
		0xC3													  // ret
	};

	BYTE srcJmp[] = {0xE9, 0x0, 0x0, 0x0, 0x0};
	BYTE stlnByte[] = {0xE9, 0x0, 0x0, 0x0, 0x0};

	DWORD stlnOff = *(DWORD*)((BYTE*)src + 1);
	DWORD stlnDst = (DWORD)((BYTE*)src + stlnOff + 5);
	uintptr_t* endGate = (uintptr_t*)((BYTE*)pGateway + sizeof(hkBytes));
    DWORD srcOff2Gate = (DWORD)pGateway - (DWORD)src  - 5;
	DWORD endGateJmp = stlnDst - (DWORD)((BYTE*)endGate + 5);

	double temp = *reinterpret_cast<double*>(dst);

	memcpy(&hkBytes[3], &dst, 8);
	memcpy(&srcJmp[1], &srcOff2Gate, 4);
	memcpy(&stlnByte[1], &endGateJmp, 4);
	memcpy(pGateway, hkBytes, 41);
	memcpy(src, srcJmp, 5);
	memcpy(endGate, stlnByte, 5);

	VirtualProtect(src, len, p, &p);
	return pGateway; // RETURTN TO AFTER
}

uintptr_t* init::Detour(uintptr_t* src, BYTE* dst, unsigned int len) {

	uintptr_t* jmp;
	DWORD p;
	uintptr_t* pGateway = nullptr;
	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &p);

	uintptr_t* pSrcLoc = src - 0x2000;
	while (!pGateway) {
		pGateway = jmp = (uintptr_t*)VirtualAlloc(pSrcLoc, 1, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE); // check VirtualAllov return
		pSrcLoc += 0x200;
	}

	DWORD oOff = *(DWORD*)((BYTE*)src + 1);
	DWORD oDst = (DWORD)((BYTE*)src + oOff + 5);

	*(BYTE*)src = 0xE9;
	*(DWORD*)((BYTE*)src + 1) = (DWORD)jmp - (DWORD)src - 5;

	BYTE JMP_ABS[6] = { 0xFF, 0x25, 0x0, 0x0, 0x0, 0x0 };
	memcpy(jmp, JMP_ABS, 7); 
	*(uintptr_t*)((BYTE*)jmp + 6) = (uintptr_t)dst - (uintptr_t)((BYTE)jmp); 

	jmp = (uintptr_t*)((BYTE*)jmp + sizeof(JMP_ABS) + sizeof(uintptr_t));
	*(BYTE*)jmp = 0xE9;

	*(DWORD*)((BYTE*)jmp + 1) = oDst - (DWORD)((BYTE*)jmp + 5);

	VirtualProtect(src, len, p, &p);
	return jmp;
}
