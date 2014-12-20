/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2013 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#ifndef XENIA_APU_AUDIO_SYSTEM_H_
#define XENIA_APU_AUDIO_SYSTEM_H_

#include <atomic>
#include <mutex>
#include <queue>
#include <thread>

#include <xenia/common.h>
#include <xenia/emulator.h>
#include <xenia/xbox.h>

namespace xe {
namespace apu {

class AudioDriver;

class AudioSystem {
 public:
  virtual ~AudioSystem();

  Emulator* emulator() const { return emulator_; }
  Memory* memory() const { return memory_; }
  cpu::Processor* processor() const { return processor_; }

  virtual X_STATUS Setup();
  virtual void Shutdown();

  X_STATUS RegisterClient(uint32_t callback, uint32_t callback_arg,
                          size_t* out_index);
  void UnregisterClient(size_t index);
  void SubmitFrame(size_t index, uint32_t samples_ptr);

  virtual X_STATUS CreateDriver(size_t index, HANDLE wait_handle,
                                AudioDriver** out_driver) = 0;
  virtual void DestroyDriver(AudioDriver* driver) = 0;

  virtual uint64_t ReadRegister(uint64_t addr);
  virtual void WriteRegister(uint64_t addr, uint64_t value);

 protected:
  virtual void Initialize();

 private:
  void ThreadStart();

  static uint64_t MMIOReadRegisterThunk(AudioSystem* as, uint64_t addr) {
    return as->ReadRegister(addr);
  }
  static void MMIOWriteRegisterThunk(AudioSystem* as, uint64_t addr,
                                     uint64_t value) {
    as->WriteRegister(addr, value);
  }

 protected:
  AudioSystem(Emulator* emulator);

  Emulator* emulator_;
  Memory* memory_;
  cpu::Processor* processor_;

  std::thread thread_;
  cpu::XenonThreadState* thread_state_;
  uint32_t thread_block_;
  std::atomic<bool> running_;

  std::mutex lock_;

  static const size_t maximum_client_count_ = 8;

  struct {
    AudioDriver* driver;
    uint32_t callback;
    uint32_t callback_arg;
    uint32_t wrapped_callback_arg;
  } clients_[maximum_client_count_];
  HANDLE client_wait_handles_[maximum_client_count_];
  std::queue<size_t> unused_clients_;
};

}  // namespace apu
}  // namespace xe

#endif  // XENIA_APU_AUDIO_SYSTEM_H_
