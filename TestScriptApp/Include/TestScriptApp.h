#pragma once
#include <dr_application.h>
#include <..\..\Script\Include\dr_script_engine.h>

namespace driderSDK {

class TestScriptApp : public Application {
  virtual void onInit() override;
  virtual void onInput() override;
  virtual void onUpdate() override;
  virtual void onDraw() override;
  virtual void onDestroy() override;
  virtual void onPause() override;
  virtual void onResume() override;

public: 
  ScriptEngine * testScript = nullptr;
};

}