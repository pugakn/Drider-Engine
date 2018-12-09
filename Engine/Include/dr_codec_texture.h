#pragma once

#include "dr_engine_prerequisites.h"
#include "dr_codec.h"
#include <dr_graphics_defines.h>
#include <vector>

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
  UniqueVoidPtr
  decode(TString pathName) override;

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
  encode(TString pathName) override;

  /**
  *
  */
  bool
  encodeImage(const String pathname,
              const Int32 width,
              const Int32 height,
              const DR_FILE_FORMAT::E fileFormat,
              const std::vector<byte>* data) const;

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
  isCompatible(TString extension) override;
  
  /**
  * TEST::
  * Gets the type of the resource 
  *
  * @return
  * Returns the type of the resource
  */
  CompatibleType::E
  getType() override;
};
}