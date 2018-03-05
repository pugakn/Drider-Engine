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
    Object(){}

    Object(float p1, float p2) {
      a = p1;
      b = p2;
    }

    Object(const Object& other) {
      a = other.a;
      b = other.b;
    }
    
    Object&
    add(const Object& param1) {
      this->a += param1.a;
      this->b += param1.b;
      return *this;
    }

    Object
    operator+(const Object& param1) const{
      return Object(a + param1.a, b + param1.b);
    }

    Object&
    operator=(const Object& param1) {
      a = param1.a;
      b = param1.b;
      return *this;
    }

    Object&
      operator+=(const Object& param1) {
      a += param1.a;
      b += param1.b;
      return *this;
    }

    Object&
    si() {
      a = 10;
      b = 11;
      return *this;
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