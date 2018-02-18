#pragma once
#include <unordered_map>
#include <vector>
#include <dr_util_prerequisites.h>
#include <dr_d3d_graphics_api.h>
#include "Model3D.h"
#include "InputComponent.h"
#include <dr_camera.h>
#include <dr_viewport.h>
#include "dr_application.h"
#include "dr_quad.h"
#include <dr_resource_manager.h>
#include <dr_graph.h>
#include <dr_memory.h>
#include <dr_shader.h>

#include <Windows.h>
#include"dr_input_manager.h"
#include "dr_input_keyboard.h"
#include "dr_input_mouse.h"
#include <iostream>

//#include <FMOD\fmod.hpp>
#include <dr_fmod_sound_api.h>
#include <dr_fmod_sound.h>
#include <dr_fmod_channel.h>
#include <dr_fmod_soundSystem.h>

namespace driderSDK {

class Technique;
  
class TestApplication : public Application
{
public:
  TestApplication();
  ~TestApplication();

  void onInit() override;
  void onInput() override;
  void onUpdate() override;
  void onDraw() override;
  void onDestroy() override;
  void onPause() override;
  void onResume() override;

  void initInput();
  void initResources();
  void initSound();
  void initSceneGraph();
  
  GraphicsAPI* driver;
  Quad quad;
  std::vector<Model3D> models;
  Viewport viewport;
  std::shared_ptr<Camera> m_camera;

  //SoundAPI* soundDriver;

  //DrSound *sound1;
  //DrSound *sound2;
  //DrChannel *channel;
  
  /*FMOD::System     *system;
  FMOD::Sound      *sound1;
  FMOD::Channel    *channel = 0;
  FMOD_RESULT       result;
  unsigned int      version;*/
  std::vector<std::unique_ptr<Technique>> m_techniques;
  std::unique_ptr<SceneGraph> m_sceneGraph;
};

}

