#pragma once
#include "dr_engine_prerequisites.h"
#define STB_IMAGE_IMPLEMENTATION
#include <STB\stb-master\stb_image.h>
#include <comdef.h>

#include "dr_imageInfo.h"
#include "dr_codec.h"

namespace driderSDK {

class DR_ENGINE_EXPORT CodecTexture : public Codec
{
 public:
  /**
  *
  */
  CodecTexture() {};

  /**
  *
  */
  virtual ~CodecTexture() {};

  /**
  * TEST::decode
  * Decodes and load a resource.
  *
  * @param pathName
  *   Path of the file (include extension).
  *
  * @return
  *   The type of the resource ResourceType::E.
  */
  void*
  decode(TString pathName);

  /**
  * TEST::encode
  * Encode and save a resource.
  *
  * @param pathName
  *   Path of the file (include extension).
  *
  * @return
  *   Shared_ptr to a Resource.
  */
  bool
  encode(TString pathName);

  /**
  * TEST::isC
  * Checks if a extension is compatible
  *
  * @param resourceName
  * The name of the resource, includ extension
  *
  * @return
  * Returns true if the resource is compatible, otherwise return false
  */
  bool
  isCompatible(TString resourceName);
};
}