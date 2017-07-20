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

#ifndef SKLAND_CORE_DEFINES_HPP_
#define SKLAND_CORE_DEFINES_HPP_

#define SKLAND_DISABLE_COPY_CONSTRUCTOR(CLASS) CLASS(const CLASS&) = delete
#define SKLAND_DISABLE_COPY_ASSIGNMENT(CLASS) CLASS& operator=(const CLASS&) = delete
#define SKLAND_DISABLE_MOVE_CONSTRUCTOR(CLASS) CLASS(CLASS&&) = delete
#define SKLAND_DISABLE_MOVE_ASSIGNMENT(CLASS) CLASS& operator=(CLASS&&) = delete

#define SKLAND_DECLARE_NONCOPYABLE(CLASS) \
  CLASS (const CLASS&) = delete; \
  CLASS& operator = (const CLASS&) = delete

#define SKLAND_DECLARE_NONMOVABLE(CLASS) \
  CLASS (CLASS&&) = delete; \
  CLASS& operator = (CLASS&&) = delete

#define SKLAND_DECLARE_NONCOPYABLE_AND_NONMOVALE(CLASS) \
  CLASS (const CLASS&) = delete; \
  CLASS& operator = (const CLASS&) = delete; \
  CLASS (CLASS&&) = delete; \
  CLASS& operator = (CLASS&&) = delete

#ifdef SKLAND_SHARED_EXPORT
#define SKLAND_EXPORT __attribute__((visibility("default")))
#define SKLAND_NO_EXPORT __attribute__((visibility("hidden")))
#else
#define SKLAND_EXPORT
#define SKLAND_NO_EXPORT
#endif  // SKLAND_SHARED_EXPORT

#ifndef SKLAND_DEPRECATED
#define SKLAND_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef SKLAND_DEPRECATED_EXPORT
#define SKLAND_DEPRECATED_EXPORT SKLAND_EXPORT SKLAND_DEPRECATED
#endif

#ifndef SKLAND_DEPRECATED_NO_EXPORT
#define SKLAND_DEPRECATED_NO_EXPORT SKLAND_NO_EXPORT SKLAND_DEPRECATED
#endif

#endif // SKLAND_CORE_DEFINES_HPP_
