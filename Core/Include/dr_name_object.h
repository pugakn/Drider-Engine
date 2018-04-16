#pragma once

#include "dr_core_prerequisites.h"

namespace driderSDK {

class DR_CORE_EXPORT NameObject
{
public:

  NameObject() = default;

  NameObject(const NameObject&) = default;

  NameObject(NameObject&&) = default;

  NameObject(const TString& _name);

  NameObject& operator=(const NameObject&) = default;

  NameObject& operator=(NameObject&&) = default;
  /**
  * Sets the name of the node
  *
  * @param name
  *  The new name.
  */
  virtual void 
  setName(const TString& name);

  const TString& 
  getName() const;
private:
  TString m_name;
};

}