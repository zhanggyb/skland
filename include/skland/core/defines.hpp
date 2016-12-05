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

//#ifdef __UNIX__
#include <string.h>
#define FILE_BASE_NAME (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
//#else
//#include <string.h>
//#define FILE_BASE_FILENAME (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
//#endif

#ifdef DEBUG

#include <stdio.h>
#define DBG_PRINT_MSG(fmt, args...)             \
    do { \
        fprintf(stderr, "%s:%d:%s(): " fmt, FILE_BASE_NAME, __LINE__, __FUNCTION__, args); \
    } while (0)

#define DBG_PRINT_PLAIN_MSG(fmt, args...)             \
    do { \
        fprintf(stderr, fmt, args); \
    } while (0)

#define DBG_SET_NAME(obj, str)                  \
    do {                                        \
      obj->set_name(str);                       \
    } while (0)

#include <cassert>
#define DBG_ASSERT(expr)                        \
  do {                                          \
    assert(expr);                               \
  } while (0)

#else // NOT DEBUG

#define DBG_PRINT_MSG(fmt, args...) ((void)0)
#define DBG_PRINT_PLAIN_MSG(fmt, args...) ((void)0)
#define DBG_SET_NAME(obj, str) ((void)0)
#define DBG_ASSERT(expr) ((void)0)

#endif // END DEBUG

#endif // SKLAND_CORE_DEFINES_HPP_
