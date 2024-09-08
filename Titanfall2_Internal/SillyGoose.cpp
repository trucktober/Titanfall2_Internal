#pragma once
#include "Includes.h"

// ----------- Window Setup ------------

struct wnd_callback {
	unsigned long proc;
	HWND hWnd;
};

HWND initi_D3D::hWnd_CW() {
	T_hWnd = CreateWindowEx(NULL, L"Fortnite", L"Battle Royale", NIS_HIDDEN, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	return T_hWnd;
}

HWND initi_D3D::hWnd_GW(unsigned long procid) {
	wnd_callback wnd;
	wnd.proc = procid;
	EnumWindows(hWnd_GCW, (LPARAM)&wnd);
	M_hWnd = wnd.hWnd;
}

BOOL CALLBACK hWnd_GCW(HWND hWnd, LPARAM wind) {
	wnd_callback wnd = *(wnd_callback*)wind;
	unsigned long procid = 0;
	GetWindowThreadProcessId(hWnd, &procid);
	if (wnd.proc != procid || !hWnd_MW(hWnd)) return TRUE;
	wnd.hWnd = hWnd;
	return FALSE;
}

bool hWnd_MW(HWND hWnd) {
	return GetWindow(hWnd, GW_OWNER) == (HWND)0 && IsWindowVisible(hWnd);
}

// ----------- Detour Setup ------------
bool initi_D3D::d3dC_CD(void** func_loc) {

	DXGI_SWAP_CHAIN_DESC scd{ 0 };
	scd.BufferCount = 1;
	scd.BufferDesc.Width = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferUsage = D3D11_USAGE_DEFAULT;
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.OutputWindow = hWnd_CW();
	scd.Windowed = TRUE;

	D3D_FEATURE_LEVEL feat_lvls[3] = { D3D_FEATURE_LEVEL_10_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_11_0 };

	if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, feat_lvls, 3, D3D11_SDK_VERSION, &scd, &sc, &dev, NULL, NULL) != S_OK)
		scd.Windowed = FALSE;
		debug = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, feat_lvls, 3, D3D11_SDK_VERSION, &scd, &sc, &dev, NULL, NULL);
		if (FAILED(debug)) {
			logger->PrintHR(debug);
			return false;
		}

	void** cur_func_loc = *reinterpret_cast<void***>(&sc);
	func_loc[0] = cur_func_loc[8];
	sc->Release();
	dev->Release();
	return true;

}

// ----------- Setup ------------
bool initi_D3D::d3dC_IA() {
	
	// Input assembler (shaderz)
}


bool initi_D3D::d3dC_RTV() {
	sc->GetDevice(__uuidof(ID3D11Device), (void**)&dev);
	dev->CreateBuffer(nullptr, 0, &backBuff)
}

bool initi_D3D::d3dC_IL() {
}

bool initi_D3D::d3dC_VP() {
	UINT count = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
	D3D11_VIEWPORT t_view_prt[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE]{};
	dev_con->RSGetViewports(&count, t_view_prt);
	if (!t_view_prt || t_view_prt->Width == 0) {
		logger->PrintError(L"GetViewports FAILED (fucking idiot lmao)");
			// get screen rect
		t_view_prt->TopLeftX = screen.left;
		t_view_prt->TopLeftY = screen.top;
		t_view_prt->Width = screen.right;
		t_view_prt->Height = screen.bottom;
		t_view_prt->MinDepth = 0.f;
		t_view_prt->MaxDepth = 1.f;
	}

	view_prt = t_view_prt[0];
	dev_con->RSSetViewports(1, &view_prt);
}

bool initi_D3D::d3dC_ORTHO() {
	ConstantBuffer cb;
	cb.proj = DirectX::XMMatrixOrthographicOffCenterLH(0.f, view_prt.Width, view_prt.Height, 0.f, 0.f, 0.f);

}


// ----------- Initialization ------------
void initi_D3D::d3dI_GSC() {

}

void initi_D3D::d3dI_GSR() {
	
}

void initi_D3D::d3dI_Rlse() {
	if (dev != nullptr) dev->Release();
	if (dev_con != nullptr) dev_con->Release();
	if (sc != nullptr) sc->Release();
	if (rtv != nullptr) rtv->Release();
	if (in_layout != nullptr) in_layout->Release();
	if (vertBuff != nullptr) vertBuff->Release();
	if (pixlBuff != nullptr) pixlBuff->Release();
	if (cnstBuff != nullptr) cnstBuff->Release();
	if (pixlBuff != nullptr) pixlBuff->Release();
	if (backBuff != nullptr) backBuff->Release();
	if (vertShad != nullptr) vertShad->Release();
	if (pixlShad != nullptr) pixlShad->Release();
}

bool initi_D3D::d3dI_Start() {

}

// ----------- Maintenance ------------
void initi_D3D::d3dR_ORTHO() {

}

