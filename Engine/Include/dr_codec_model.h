#pragma once

#include "dr_engine_prerequisites.h"
#include "dr_codec.h"

namespace driderSDK {

class DR_ENGINE_EXPORT CodecModel : public Codec
{
 public:
  /** 
  * Default constructor
  */
  CodecModel() {}

  /**
  * Destructor
  */
  ~CodecModel() {}

 private:
  
  virtual UniqueVoidPtr 
  decode(TString pathName) override;

  virtual bool 
  encode(TString pathName) override;

  virtual bool 
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