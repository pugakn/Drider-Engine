#pragma once
#include <dr_util_prerequisites.h>

#include<vector>
namespace driderSDK {
class InputEditor {
 public:
  /**
  * Return to inputs in component
  *
  * @param response
  *
  * @return
  *   TString whit format json for de front end
  */
  virtual void
  getInputs(TString* response) = 0;

  /**
  * Add input for diferents types:
  *   droppableFile - values: nameFile
  *   droppableGO - values: nameGameObjects
  *   number
  *   text: 
  *   color - values: #ff0000
  *   checkbox - values: true, false
  */
  static TString
  addInput(TString id,
           TString type,
           TString name,
           TString value);

  /**
  * Add input type selectable
  *   selected: option selected
  *   options: diferent options
  */
  static TString
  addInputSelectable(TString id,
                     TString name,
                     TString selected,
                     std::vector<TString> *options);

};
}