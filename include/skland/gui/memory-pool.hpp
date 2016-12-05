/*
 * Copyright 2016 Freeman Zhang <zhanggyb@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SKLAND_GUI_SHM_POOL_HPP_
#define SKLAND_GUI_SHM_POOL_HPP_

#include "shared-memory.hpp"

#include <sys/types.h>

#include <skland/wayland/client/shm-pool.hpp>

namespace skland {

/**
 * @brief Shared memory pool
 */
class MemoryPool {

  MemoryPool &operator=(const MemoryPool &) = delete;
  MemoryPool(const MemoryPool &) = delete;

 public:

  MemoryPool()
      : size_(0) {}

  /**
   * @brief Destructor
   *
   * Destroy the pool, this does not unmap the memory though.
   */
  inline ~MemoryPool() {}

  void Setup(int32_t size);

  void Destroy();

  int32_t size() const {
    return size_;
  }

  const wayland::client::ShmPool &wl_shm_pool() const {
    return wl_shm_pool_;
  }

  const SharedMemory::SharedPtr &data() const {
    return data_;
  }

 private:

  static int CreateAnonymousFile(off_t size);

  static int CreateTmpfileCloexec(char *tmpname);

  wayland::client::ShmPool wl_shm_pool_;

  int32_t size_;
  SharedMemory::SharedPtr data_;

};

}

#endif // SKLAND_GUI_SHM_POOL_HPP_
