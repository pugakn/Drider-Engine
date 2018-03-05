#pragma once
#include <dr_string_utils.h>
#include <dr_application.h>
#include <dr_camera.h>
#include <dr_resource_manager.h>
#include <dr_graph.h>
#include <dr_script_engine.h>

namespace driderSDK {

class TestScriptApp : public Application {
public:
  TestScriptApp();
  ~TestScriptApp();

  void
  postInit() override;

  void
  postUpdate() override;

  void
  postRender() override;

  void
  postDestroy() override;

  void 
  input();
  void 
  initInput();
  void 
  initResources();
  void 
  initSound();
  void 
  initSceneGraph();

  void 
  initScriptEngine();

private:
  std::shared_ptr<Camera> m_camera;
  std::shared_ptr<GameObject> m_joker;
  ScriptEngine* scriptEngine;

  
};

}