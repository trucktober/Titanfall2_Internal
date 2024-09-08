#pragma once

#include <Windows.h>
#include <iostream>
#include <vector>
#include <d3d11.h>
#include <d3dx11.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "hackinit.h"
#include <DirectXColors.h>
#include "shader.h"
#include "ErrorLogger.h"
#include "dx11Setup.h"

#include "SillyGoose.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
