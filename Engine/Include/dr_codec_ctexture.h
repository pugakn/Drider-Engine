#pragma once

#include "dr_engine_prerequisites.h"
#include "dr_codec.h"

namespace driderSDK {

  class DR_ENGINE_EXPORT CodecCompressedTexture : public Codec
  {
  public:
    /**
    *
    */
    CodecCompressedTexture() {};

    /**
    *
    */
    virtual ~CodecCompressedTexture() {};

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