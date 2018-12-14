#include "dr_editor.h"

int main(int, char**) {
  return driderSDK::Editor().run(driderSDK::Viewport{ 0, 0, 720, 480, 0, 0 });
}