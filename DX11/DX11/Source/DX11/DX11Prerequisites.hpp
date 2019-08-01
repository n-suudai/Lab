#pragma once


//-----------------------------------------------------------------
// Direct3D API 関連
//-----------------------------------------------------------------
#include <dxgi.h>
#include <d3d11.h>
#include <wrl.h>

template<class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

