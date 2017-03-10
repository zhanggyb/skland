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

#ifndef SKLAND_GUI_SHARED_MEMORY_POOL_HPP_
#define SKLAND_GUI_SHARED_MEMORY_POOL_HPP_

#include <sys/types.h>

#include <skland/wayland/shm-pool.hpp>

namespace skland {

/**
 * @brief Shared memory pool
 */
class SharedMemoryPool {

  SharedMemoryPool &operator=(const SharedMemoryPool &) = delete;
  SharedMemoryPool(const SharedMemoryPool &) = delete;

 public:

  SharedMemoryPool()
      : size_(0), data_(nullptr) {}

  /**
   * @brief Destructor
   *
   * Destroy the pool, this does not unmap the memory though.
   */
  inline ~SharedMemoryPool() {}

  void Setup(int32_t size);

  void Destroy();

  int32_t size() const {
    return size_;
  }

  const wayland::ShmPool &wl_shm_pool() const {
    return wl_shm_pool_;
  }

  void *data() const { return data_; };

 private:

  static int CreateAnonymousFile(off_t size);

  static int CreateTmpfileCloexec(char *tmpname);

  wayland::ShmPool wl_shm_pool_;

  int32_t size_;

  void *data_;

};

}

#endif // SKLAND_GUI_SHARED_MEMORY_POOL_HPP_
