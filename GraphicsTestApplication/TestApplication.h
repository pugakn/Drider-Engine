#pragma once
#include <vector>
#include <dr_util_prerequisites.h>
#include <dr_d3d_graphics_api.h>
#include "Model3D.h"
#include <dr_camera.h>
#include <dr_viewport.h>
#include "dr_application.h"
#include "dr_quad.h"
#include <dr_resource_manager.h>


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
#include <dr_sound_core.h>

namespace driderSDK {

class KeyboardListener : public driderSDK::IKeyboardListener
{
 public:
  void 
  setSoundDriver(SoundAPI* soundDriver) {
    this->soundDriver = soundDriver;
  }

  void 
  setResourceManager(ResourceManager* resourceManager) {
    this->resourceManager = resourceManager;
  }

  void
  setCamera(Camera *camera) {
    this->camera = camera;
  }

  
  bool
  keyPressed(const KeyboardButtonID::E& key) override {
    /*exit(666);
    std::cout << "World ends here..." << std::endl;*/
    if(key == KeyboardButtonID::KC_1) {
      bool paused;
      soundDriver->channel1->getPaused(&paused);
      std::cout << "Channel 1: "<< "paused: " << paused << std::endl;
      soundDriver->channel1->setPaused(paused == false ? true : false);
    } 
    else if (key == KeyboardButtonID::KC_2) {
      auto soundResource = resourceManager->getReference(_T("testSound2.mp3"));
      std::cout << "Add sound2 in channel 2." << std::endl;
      auto sound = std::dynamic_pointer_cast<SoundCore>(soundResource);
      sound->get()->play();
    } 
    else if (key == KeyboardButtonID::KC_3) {
      auto soundResource = resourceManager->getReference(_T("testSound3.mp3"));
      std::cout << "Add sound3 in channel 3. " << std::endl;
      auto sound = std::dynamic_pointer_cast<SoundCore>(soundResource);
      sound->get()->play();
    }
    else if (key == KeyboardButtonID::KC_UP) {
      float volume;
      soundDriver->channel1->getVolume(&volume);
      volume += 0.1f;
      if(volume < 1.0f) {
        soundDriver->channel1->setVolume(volume);
      } else {
        volume = 1.0f;
      }
      std::cout << "Channel1 volume: " << volume << std::endl;
    }
    else if (key == KeyboardButtonID::KC_DOWN) {
      float volume;
      soundDriver->channel1->getVolume(&volume);
      volume -= 0.1f;
      if (volume > 0.0f) {
        soundDriver->channel1->setVolume(volume);
      }
      else {
        volume = 0.0f;
        soundDriver->channel1->setVolume(volume);
      }
      std::cout << "Channel1 volume: " << volume << std::endl;
    }

    return true;
  }

  bool
  keyReleased(const KeyboardButtonID::E& key) override {
    /*exit(666);
    std::cout << "World ends here..." << std::endl;*/
    return false;
  }

  SoundAPI* soundDriver;
  ResourceManager* resourceManager;
  Camera* camera;
};

class MouseTest : public driderSDK::IMouseInputListener
{
 public:
  virtual bool
  mouseMoved(const MouseInputState& state) {
    //std::cout << state.m_cursorPosition.x << " , " << state.m_cursorPosition.y << std::endl;
    return true;
  }

  virtual bool
  mousePressed(const MouseInputState& state, MouseButtonID::E pressedId) {
    /*exit(666);
    std::cout << "World ends here..." << std::endl;*/
    return true;
  }

  virtual bool
  mouseReleased(const MouseInputState& state, MouseButtonID::E pressedId) {
    /*exit(666);
    std::cout << "World ends here..." << std::endl;*/
    return true;
  }
};

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


  GraphicsAPI* driver;
  Quad quad;
  std::vector<Model3D> models;
  Viewport viewport;
  Camera camera;

  SoundAPI* soundDriver;
  ResourceManager* resourceManager;

  InputManager m_inputManager;
  MouseInput* m_mouseInput;
  MouseTest m_mouseListener;
  KeyboardInput *m_keyboardInput;
  KeyboardListener m_keyboardListener;

  float velMove; 
  
};

}

