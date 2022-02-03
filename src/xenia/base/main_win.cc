/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2022 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#include <malloc.h>
#include <cstring>

#include "xenia/base/cvar.h"
#include "xenia/base/logging.h"
#include "xenia/base/main_win.h"
#include "xenia/base/platform_win.h"
#include "xenia/base/string.h"

// Autogenerated by `xb premake`.
#include "build/version.h"

// For RequestWin32MMCSS.
#include <dwmapi.h>
// For RequestWin32HighResolutionTimer.
#include <winternl.h>

DEFINE_bool(win32_high_resolution_timer, true,
            "Requests high-resolution timer from the NT kernel", "Win32");
DEFINE_bool(
    win32_mmcss, true,
    "Opt in the Multimedia Class Scheduler Service (MMCSS) scheduling for "
    "prioritized access to CPU resources",
    "Win32");

namespace xe {

static void RequestWin32HighResolutionTimer() {
  HMODULE ntdll_module = GetModuleHandleW(L"ntdll.dll");
  if (!ntdll_module) {
    return;
  }

  // clang-format off
  NTSTATUS (NTAPI* nt_query_timer_resolution)(OUT PULONG MinimumResolution,
                                              OUT PULONG MaximumResolution,
                                              OUT PULONG CurrentResolution);
  NTSTATUS (NTAPI* nt_set_timer_resolution)(IN ULONG DesiredResolution,
                                            IN BOOLEAN SetResolution,
                                            OUT PULONG CurrentResolution);
  // clang-format on
  nt_query_timer_resolution =
      reinterpret_cast<decltype(nt_query_timer_resolution)>(
          GetProcAddress(ntdll_module, "NtQueryTimerResolution"));
  nt_set_timer_resolution = reinterpret_cast<decltype(nt_set_timer_resolution)>(
      GetProcAddress(ntdll_module, "NtSetTimerResolution"));
  if (!nt_query_timer_resolution || !nt_set_timer_resolution) {
    return;
  }

  ULONG minimum_resolution, maximum_resolution, current_resolution;
  nt_query_timer_resolution(&minimum_resolution, &maximum_resolution,
                            &current_resolution);
  nt_set_timer_resolution(maximum_resolution, TRUE, &current_resolution);
}

static void RequestWin32MMCSS() {
  HMODULE dwmapi_module = LoadLibraryW(L"dwmapi.dll");
  if (!dwmapi_module) {
    return;
  }
  // clang-format off
  HRESULT (STDAPICALLTYPE* dwm_enable_mmcss)(BOOL fEnableMMCSS);
  // clang-format on
  dwm_enable_mmcss = reinterpret_cast<decltype(dwm_enable_mmcss)>(
      GetProcAddress(dwmapi_module, "DwmEnableMMCSS"));
  if (dwm_enable_mmcss) {
    dwm_enable_mmcss(TRUE);
  }
  FreeLibrary(dwmapi_module);
}

bool ParseWin32LaunchArguments(
    bool transparent_options, const std::string_view positional_usage,
    const std::vector<std::string>& positional_options,
    std::vector<std::string>* args_out) {
  auto command_line = GetCommandLineW();

  int wargc;
  wchar_t** wargv = CommandLineToArgvW(command_line, &wargc);
  if (!wargv) {
    return false;
  }

  // Convert all args to narrow, as cxxopts doesn't support wchar.
  int argc = wargc;
  char** argv = reinterpret_cast<char**>(alloca(sizeof(char*) * argc));
  for (int n = 0; n < argc; n++) {
    size_t len = std::wcstombs(nullptr, wargv[n], 0);
    argv[n] = reinterpret_cast<char*>(alloca(sizeof(char) * (len + 1)));
    std::wcstombs(argv[n], wargv[n], len + 1);
  }

  LocalFree(wargv);

  if (!transparent_options) {
    cvar::ParseLaunchArguments(argc, argv, positional_usage,
                               positional_options);
  }

  if (args_out) {
    args_out->clear();
    for (int n = 0; n < argc; n++) {
      args_out->push_back(std::string(argv[n]));
    }
  }

  return true;
}

int InitializeWin32App(const std::string_view app_name) {
  // Initialize logging. Needs parsed FLAGS.
  xe::InitializeLogging(app_name);

  // Print version info.
  XELOGI(
      "Build: "
#ifdef XE_BUILD_IS_PR
      "PR#" XE_BUILD_PR_NUMBER " " XE_BUILD_PR_REPO " " XE_BUILD_PR_BRANCH
      "@" XE_BUILD_PR_COMMIT_SHORT " against "
#endif
      XE_BUILD_BRANCH "@" XE_BUILD_COMMIT_SHORT " on " XE_BUILD_DATE);

  // Request high-performance timing and scheduling.
  if (cvars::win32_high_resolution_timer) {
    RequestWin32HighResolutionTimer();
  }
  if (cvars::win32_mmcss) {
    RequestWin32MMCSS();
  }

  return 0;
}

void ShutdownWin32App() { xe::ShutdownLogging(); }

}  // namespace xe
