#pragma once

#include "imgui.h"
#include "dr_file_system.h"
#include <dr_string_utils.h>

#define IMGUIHELPER_H_

#ifdef IMGUIHELPER_H_

namespace ImGuiHelper {

using namespace driderSDK;

struct Serializer {

  Serializer(const char* filename) {
    FileSystem fileSystem;
    fileSystem.CreateAndOpen(StringUtils::toTString(filename), file);
  }

  bool
  isValid() {
    return true;
  }

  void
  save(int* sz,
       const char* prop) {
    file.m_file << prop << "\n";
    file.m_file << sz << "\n";
  }

  void
  save(float* sz,
       const char* prop) {
    file.m_file << prop << "\n";
    file.m_file << sz << "\n";
  }

  void
  save(char* sz,
       const char* prop) {
    file.m_file << prop << "\n";
    file.m_file << sz << "\n";
  }

  void
  save(float* sz,
       const char* prop,
       int i) {
    file.m_file << prop << "\n";
    file.m_file << sz << "\n";
  }

  File file;
};

#endif // IMGUIHELPER_H_
