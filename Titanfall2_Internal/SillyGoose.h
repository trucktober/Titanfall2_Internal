#pragma once
#include "Includes.h"



class initi_D3D {
private:
	ID3D11Device* dev;
	ID3D11DeviceContext* dev_con;
	IDXGISwapChain* sc;
	ID3D11RenderTargetView* rtv;
	D3D11_VIEWPORT view_prt;
	ID3D11InputLayout* in_layout;

	ID3D11Buffer* vertBuff;
	ID3D11Buffer* pixlBuff;
	ID3D11Buffer* cnstBuff;
	ID3D11Buffer* backBuff;
	ID3D11VertexShader* vertShad;
	ID3D11PixelShader* pixlShad;
	
	RECT screen;
	HWND T_hWnd, M_hWnd;
	HRESULT debug;
	ErrorLogger* logger;
public:
// Structor
	initi_D3D(IDXGISwapChain* sc) : sc(sc), debug(NULL), logger(ErrorLogger::getinst()) {};
	~initi_D3D() {};
	initi_D3D operator=(const initi_D3D) = delete;
	initi_D3D(initi_D3D&) = delete;

// Window Setup
	HWND hWnd_CW();
	HWND hWnd_GW(unsigned long procid);

// Detour Setup
	bool d3dC_CD(void** func_loc);

// Setup
	bool d3dC_IA();
	bool d3dC_RTV();
	bool d3dC_IL();
	bool d3dC_VP();
	bool d3dC_ORTHO();

// Initialization
	void d3dI_GSC(); // "Game State Capture
	void d3dI_GSR(); // "Game State Restore
	void d3dI_Rlse();
	bool d3dI_Start();

// Maintenance
	void d3dR_ORTHO();

// Structures

	struct ConstantBuffer {
		DirectX::XMMATRIX proj;
	};

	struct Vertex {
		DirectX::XMFLOAT3 pos;
		D3DCOLORVALUE color;
	};
};