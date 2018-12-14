#include "dr_serializable_camera.h"

#include <dr_gameObject.h>
#include <dr_camera_component.h>

namespace driderSDK {
void
sCamera::load(File &file,
              std::shared_ptr<GameObject> obj) {
  String name;
  file.m_file >> name;

  obj->createComponent<CameraComponent>();

  
}

}