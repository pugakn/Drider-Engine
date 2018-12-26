#pragma once

#include <vector>

#include "dr_util_prerequisites.h"

namespace driderSDK {

class DR_UTIL_EXPORT FileDialog
{
  public:
    static TString 
    getOpenFileName(const TString& title, const TString& filter, const TString& dir = TString());

    static TString
    getSaveFileName(const TString& title, const TString& filter, const TString& dir = TString());
    
  private:
};

}
