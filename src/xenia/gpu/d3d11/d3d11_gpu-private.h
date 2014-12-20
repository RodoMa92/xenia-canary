/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2013 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#ifndef XENIA_GPU_D3D11_D3D11_GPU_PRIVATE_H_
#define XENIA_GPU_D3D11_D3D11_GPU_PRIVATE_H_

#include <d3d11.h>

#include <xenia/common.h>
#include <xenia/gpu/d3d11/d3d11_gpu.h>

namespace xe {
namespace gpu {
namespace d3d11 {

template <typename T>
void SafeRelease(T* ptr) {
  if (ptr) {
    ptr->Release();
  }
}

}  // namespace d3d11
}  // namespace gpu
}  // namespace xe

#endif  // XENIA_GPU_D3D11_D3D11_GPU_PRIVATE_H_
