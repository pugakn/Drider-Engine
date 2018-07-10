#pragma once

#include <dr_memory.h>
#include <dr_util_prerequisites.h>

namespace driderSDK {

class GameObject;

class Spawner
{
public:

  using SharedObj = std::shared_ptr<GameObject>;

  void
  setModel(SharedObj model);

  SharedObj
  spawn();
private:

  SharedObj m_original;
};

}
