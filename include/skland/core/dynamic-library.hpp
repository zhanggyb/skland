//
// Created by zhanggyb on 17-2-8.
//

#ifndef SKLAND_CORE_DL_HPP_
#define SKLAND_CORE_DL_HPP_

#include <dlfcn.h>

namespace skland {

class DynamicLibrary {

 public:

  DynamicLibrary()
      : handle_(nullptr) {}

  ~DynamicLibrary();

  void Open(const char *filename, int flags);

  void Close();

  void *GetSymbol(const char *symbol);

  bool IsValid() const { return nullptr != handle_; }

 private:

  void *handle_;

};

}

#endif //SKLAND_DL_HPP
