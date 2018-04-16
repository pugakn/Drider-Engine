#pragma once

#include <dr_application.h>
#include <dr_memory.h>
#include <dr_util_prerequisites.h>
#include <dr_timer.h>

namespace driderSDK {

class GameObject;
class Model;
class Technique;

class TestApplication : public Application
{
 public:
  TestApplication();
  ~TestApplication();

  void 
  postInit() override;

  void
  postUpdate() override;

  void 
  postRender() override;

  void
  postDestroy() override;

  void
  initModules();

  void
  initInputCallbacks();

  void 
  createTechniques();

  void
  loadResources();

  void
  createScene();

  std::shared_ptr<GameObject>
  addObjectFromModel(std::shared_ptr<Model> model,
                     const TString& name);

  void
  initScriptEngine();

  void
  destroyModules();

  Int32 m_currCam;
  TString m_camNames[2];

  std::shared_ptr<GameObject> crocObj;

  std::unique_ptr<Technique> m_staticTech;
  std::unique_ptr<Technique> m_animTech;

  //DrSound *sound1;
  //DrSound *sound2;
  //DrChannel *channel;
  
  /*FMOD::System     *system;
  FMOD::Sound      *sound1;
  FMOD::Channel    *channel = 0;
  FMOD_RESULT       result;
  unsigned int      version;*/

};

}

