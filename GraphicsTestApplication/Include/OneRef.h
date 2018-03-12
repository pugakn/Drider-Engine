#pragma once
#include <dr_util_prerequisites.h>

namespace driderSDK {

class OneRef {
  public:
    OneRef();

    ~OneRef();

    void
    move(float x);

    float posX;

};

}