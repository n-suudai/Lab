#pragma once


// プリコンパイル済みヘッダー


//-----------------------------------------------------------------
// External 関連
//-----------------------------------------------------------------
#pragma warning(push)
#pragma warning(disable:4127)
#pragma warning(disable:4201)
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/color_space.hpp>
#pragma warning(pop)

#include "lodepng/lodepng.h"
#include "imgui/imgui.h"
#include "imgui/examples/imgui_impl_dx11.h"
#include "imgui/examples/imgui_impl_win32.h"


//-----------------------------------------------------------------
// Windows 関連
//-----------------------------------------------------------------
#if !defined(STRICT)
#   define STRICT
#endif

#if !defined(WIN32_LEAN_AND_MEAN)
#   define WIN32_LEAN_AND_MEAN
#endif

#if !defined(NOMINMAX) && defined(_MSC_VER)
#   define NOMINMAX
#endif

#include <tchar.h>
#include <Windows.h>


//-----------------------------------------------------------------
// Direct3D API 関連
//-----------------------------------------------------------------
#include <dxgi.h>
#include <d3d11.h>
#include <wrl.h>


template<class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;


//-----------------------------------------------------------------
// STL 関連
//-----------------------------------------------------------------
#if _HAS_ITERATOR_DEBUGGING > 0
#   undef _HAS_ITERATOR_DEBUGGING
#   define _HAS_ITERATOR_DEBUGGING 0
#endif

#include <cstdint>
#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include <fstream>
#include <iterator>
#include <thread>
#include <algorithm>


//-----------------------------------------------------------------
// typedef
//-----------------------------------------------------------------
typedef std::int8_t  s8;
typedef std::int16_t s16;
typedef std::int32_t s32;
typedef std::int64_t s64;

typedef std::uint8_t  u8;
typedef std::uint16_t u16;
typedef std::uint32_t u32;
typedef std::uint64_t u64;

typedef float  f32;
typedef double f64;



//-----------------------------------------------------------------
// アプリケーション関連
//-----------------------------------------------------------------
#include "KeyCode.hpp"
#include "App.hpp"
#include "AppWin.hpp"
#include "Sample/ResultUtil.hpp"
#include "Sample/Graphics/Texture.hpp"
#include "Sample/Vertex.hpp"
#include "Sample/Font/BitmapFont.hpp"
#include "Sample/Shape/Shape.hpp"
#include "Sample/SampleApp.hpp"
#include "Sample/Utility.hpp"

