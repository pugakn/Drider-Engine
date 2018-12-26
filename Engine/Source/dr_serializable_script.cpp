#include "dr_serializable_script.h"

#include <dr_file.h>
#include <dr_gameObject.h>
#include <dr_script_component.h>
#include <dr_resource_manager.h>
#include <dr_script_core.h>

namespace driderSDK {

void
sScript::load(File &file,
              std::shared_ptr<GameObject> obj) {
  String componentName;
  file.m_file >> componentName;

  String scriptName;
  file.m_file >> scriptName;
  
  if(scriptName == "null")  {
    auto component = obj->createComponent<ScriptComponent>();
    component->setName(StringUtils::toTString(componentName));
  } else {
    auto scriptResource = ResourceManager::loadResource(StringUtils::toTString(scriptName));
    auto script = std::dynamic_pointer_cast<ScriptCore>(scriptResource);
    auto component = obj->createComponent<ScriptComponent>(script);
    //component->setName(StringUtils::toTString(componentName));
    ResourceManager::insertCompilableScript(component);
  }
    
}
}