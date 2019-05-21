#include "dr_editor.h"

driderSDK::Int32
main(driderSDK::Int32, driderSDK::ANSIChar**) {
  return driderSDK::Editor().run(driderSDK::Viewport{ 0, 0, 720, 480, 0, 0 });
}