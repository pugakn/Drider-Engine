#pragma once
#include <dr_string_utils.h>
#include <dr_application.h>
#include <dr_camera.h>
#include <dr_resource_manager.h>
#include <dr_graph.h>
#include <dr_script_engine.h>

namespace driderSDK {

class Object {
  public:
    Int32 refCount;
    Object(){}

    void addRef () {
      
    }

    void release() {
      if(--refCount == 0)
        delete this; 
    }

    Object
    add(const Object& param1) const {
      Object o;
      o.a += param1.a;
      o.b += param1.b;
      return o;
    }

  public:
    float a;
    float b;
   
};

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

  QUERY_ORDER::E m_queryOrder;

  
};

}