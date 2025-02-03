/**
******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2025 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#ifndef D3D12_LINUX_UTIL_H
#define D3D12_LINUX_UTIL_H
//Tiny library glue logic to translate Win32 Signal API -> POSIX events equivalent calls
#include "../../../third_party/pevents/src/pevents.h"
#define _In_
#define _COM_Outptr_
#define CreateEvent(a,b,c,d) neosmart::CreateEvent(b,c)
#define CloseHandle neosmart::DestroyEvent
#define IDXGraphicsAnalysis void
#endif //D3D12_LINUX_UTIL_H
