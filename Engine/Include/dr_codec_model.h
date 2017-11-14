#pragma once

#include "dr_engine_prerequisites.h"
#include "dr_codec.h"

namespace driderSDK {

class CodecModel : public Codec
{
 public:
 private:
   // Inherited via Codec
   virtual void* decode(TString pathName) override;

   virtual void encode(TString pathName, ResourceType::E resourceType) override;

   virtual bool isCompatible(TString resourceName) override;
};

}