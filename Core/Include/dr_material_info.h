#pragma once
#include "dr_core_prerequisites.h"
#include <unordered_map>

namespace driderSDK {

struct DR_CORE_EXPORT MaterialInfo
{
  MaterialInfo() {}
  /*
  name: string;
	m_proyectShadow: bool;
	num_properties: int;
	properties: vector<Propertie>;
  */
  TString name;
  std::unordered_map<String, std::unordered_map<SizeT, String>> props;
};

}