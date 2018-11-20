#include "dr_random.h"

namespace driderSDK {

std::mt19937&
Random::getEngine() {
  static std::mt19937 engine{std::random_device{}()};
  return engine;
}

}