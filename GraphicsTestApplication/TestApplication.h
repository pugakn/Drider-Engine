#pragma once
#include <unordered_map>
#include <vector>
#include <dr_util_prerequisites.h>
#include <dr_camera.h>
#include <dr_viewport.h>
#include "dr_application.h"
#include <dr_resource_manager.h>
#include <dr_memory.h>
#include <dr_shader.h>

#include <Windows.h>
#include <iostream>
#include <dr_graph.h>
#include <thread>

//#include <FMOD\fmod.hpp>
#include <dr_fmod_sound_api.h>
#include <dr_fmod_sound.h>
#include <dr_fmod_channel.h>
#include <dr_fmod_soundSystem.h>

namespace driderSDK {

class Technique;
class StaticMeshTechnique;
class AnimationTechnique;
  
class TestApplication : public Application
{
 public:
  TestApplication();
  ~TestApplication();

  void 
  postInit() override;
  /*void onInput() override;*/
  void
  postUpdate() override;

  void 
  postRender() override;
  //void onDraw() override;
  void
  postDestroy() override;
  
  void input();
  void initInput();
  void initResources();
  void initSceneGraph();
  void initOctree();

  void printHerarchy(std::shared_ptr<GameObject> obj, const TString& off);

  void toggleAABBDebug(std::shared_ptr<GameObject> obj);

  void toggleSkeletonDebug(std::shared_ptr<GameObject> obj);

  bool m_debugList;
  //SoundAPI* soundDriver;
  QUERY_ORDER::E m_queryOrder;
  //DrSound *sound1;
  //DrSound *sound2;
  //DrChannel *channel;
  
  /*FMOD::System     *system;
  FMOD::Sound      *sound1;
  FMOD::Channel    *channel = 0;
  FMOD_RESULT       result;
  unsigned int      version;*/
  std::shared_ptr<Camera> m_camera;
  std::shared_ptr<Camera> m_leftCam;
  std::shared_ptr<Camera> m_upCam;
  std::shared_ptr<Camera> m_activeCam;
  std::unique_ptr<Technique> m_technique;
  std::unique_ptr<AnimationTechnique> m_animTech;
  std::unique_ptr<Technique> m_linesTech;
  std::shared_ptr<GameObject> m_joker;
  std::shared_ptr<GameObject> m_wep;
};

}

