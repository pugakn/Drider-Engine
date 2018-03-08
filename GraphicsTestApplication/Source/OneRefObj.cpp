#include "..\Include\OneRefObj.h"

namespace driderSDK {

driderSDK::OneRefObj::OneRefObj() {
}

driderSDK::OneRefObj::~OneRefObj() {
}

bool driderSDK::OneRefObj::cmpInts(int a, int b) {
  return (a == b);
}

}
