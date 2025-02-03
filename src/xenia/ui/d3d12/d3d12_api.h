/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2018 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#ifndef XENIA_UI_D3D12_D3D12_API_H_
#define XENIA_UI_D3D12_D3D12_API_H_

#ifdef XE_PLATFORM_WIN32
// Must be included before D3D and DXGI for things like NOMINMAX.
#include "xenia/base/platform_win.h"

#include <DXProgrammableCapture.h>
#endif
#include <d3d12.h>
#include <d3d12sdklayers.h>
#include <d3dcompiler.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>
// For Microsoft::WRL::ComPtr.
#ifdef XE_PLATFORM_WIN32
#include <wrl/client.h>
#endif

#ifdef XE_PLATFORM_WIN32
#include "third_party/DirectXShaderCompiler/include/dxc/dxcapi.h"
#include "third_party/DirectXShaderCompiler/projects/dxilconv/include/DxbcConverter.h"
#endif

#define XELOGD3D XELOGI

#endif  // XENIA_UI_D3D12_D3D12_API_H_
