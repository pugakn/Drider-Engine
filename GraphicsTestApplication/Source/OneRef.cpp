#include "OneRef.h"

namespace driderSDK {

OneRef::OneRef() {
  posX = 0;
}

OneRef::~OneRef() {

}

void
OneRef::move(float x) {
  posX += x;
}

}
