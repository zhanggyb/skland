//
// Created by zhanggyb on 17-2-8.
//

#include <skland/core/dynamic-library.hpp>

#include <iostream>

using std::cerr;
using std::endl;

namespace skland {

DynamicLibrary::~DynamicLibrary() {
  if (handle_) dlclose(handle_);
}

void DynamicLibrary::Open(const char *filename, int flags) {
  Close();

  handle_ = dlopen(filename, flags);
  if (nullptr == handle_) {
    cerr << "Error! Cannot load library: " << dlerror() << endl;
  }
}

void DynamicLibrary::Close() {
  if (handle_) {
    dlclose(handle_);
    handle_ = nullptr;
  }
}

void *DynamicLibrary::GetSymbol(const char *symbol) {
  void* addr = dlsym(handle_, symbol);
  const char* error = dlerror();
  if (error) {
    cerr << "Error! Cannot load symbol: " << error << endl;
  }

  return addr;
}

}