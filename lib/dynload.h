#pragma once

#include <string>

#if WIN32
  #include <Windows.h>
  typedef HMODULE dynlib_t;

  inline dynlib_t dynLibOpen(const std::string & path) {
    return LoadLibrary(path.c_str());
  }

  inline void dynLibClose(dynlib_t lib) {
    FreeLibrary(lib);
  }

  inline void * dynLibGetSymbolPtr(dynlib_t lib, const std::string & name) {
    return GetProcAddress(lib, name.c_str());
  }
#else
  #include <dlfcn.h>
  typedef void * dynlib_t;

  inline dynlib_t dynLibOpen(const std::string & path) {
    return dlopen(path.c_str(), RTLD_LAZY);
  }

  inline void dynLibClose(dynlib_t lib) {
    dlclose(lib);
  }

  inline void * dynLibGetSymbolPtr(dynlib_t lib, const std::string & name) {
    return dlsym(lib, name.c_str());
  }
#endif
