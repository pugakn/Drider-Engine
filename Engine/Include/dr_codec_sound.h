#pragma once
#include "dr_engine_prerequisites.h"
#include "dr_codec.h"

namespace driderSDK {

class SoundSystem;

class DR_ENGINE_EXPORT CodecSound : public Codec {

 public:
  CodecSound() {}
  virtual ~CodecSound() {}

  UniqueVoidPtr
  decode(TString pathName) override;

  bool
  encode(TString pathName) override;

  bool
  isCompatible(TString resourceName) override;

  CompatibleType::E
  getType() override;

};

}