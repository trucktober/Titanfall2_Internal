#include "Includes.h"
init* init::inst = nullptr;
Device* Device::instance = nullptr;
ErrorLogger* ErrorLogger::instance = nullptr;

HRESULT APIENTRY hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
    init* instan = init::getinst();
    Device* d = Device::getinst();
    ErrorLogger* logger = ErrorLogger::getinst();

    if (!d->sc) d->sc = pSwapChain;
    
    
  //  d->imguitest(d->sc); //trace the imgui renderng and see how they dont mess everything up lol
    

    if (!d->d3dinti) {
       // d->Startup();
        d->GetD3D11(d->sc);
       // d->Shutdown();
    }
  //  d->ClearScreen();
    d->DrawTriangle();

    logger->PrintError(L"In hkPresent");
    return instan->oPresent(pSwapChain, SyncInterval, Flags);
}

BOOL WINAPI HackThread(HMODULE hModule) {
    init* instance = init::getinst();
    instance->enable();
   
    while (!GetAsyncKeyState(VK_END)) {
        Sleep(50);
       
    }

    FreeLibraryAndExitThread(hModule, 0);
}



BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  reason,
                       LPVOID lpReserved
                     )
{
    DisableThreadLibraryCalls(hModule);
    if (reason == 1) {
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr));
    }

    return TRUE;
}

