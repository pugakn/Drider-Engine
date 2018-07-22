#pragma once
#include <dr_util_prerequisites.h>

#include <dr_application.h>
#include <dr_timer.h>
#include <dr_renderman.h>
#include "dr_client.h"

#include "dr_scene_editor.h"
#include <dr_web_renderer.h>

namespace driderSDK {

class GameObject;
class Model;
class SoundExtraInfo;
class ScriptComponent;

class DriderEngine : public Application, 
                     public Client
{
public:

  DriderEngine() : Application() {}

  ~DriderEngine();

protected:
  virtual void
  onJoinAccepted() override;

  virtual void
  onJoinDenied() override;

  virtual void
  onConnectionLoss() override;

  virtual void
  onLobbiesListReceived(LobbiesList&& lobbies) override;

  virtual void
  onGameStatusReceived(WString&& msg) override;

private:
  // Inherited via Application
  virtual void
  postInit() override;

  virtual void
  postUpdate() override;

  virtual void
  postRender() override;

  virtual void
  postDestroy() override;

  void
  initModules();

  void
  loadResources();

  void
  createScene();

  std::shared_ptr<GameObject>
  addObjectFromModel(std::shared_ptr<Model> model,
                     const TString& name);

  void
  destroyModules();

  void
  initScriptEngine();

  void
  playSoundTest();

  // Inherited via Application
  virtual void
  onResize() override;

 private:
  SceneEditor m_editor;
  RenderMan m_renderMan;

  SoundExtraInfo* extraInfo;
  std::unordered_map<TString, ScriptComponent*> m_scripts;

  std::shared_ptr<GameObject> m_player;

  GameObject* m_root;

  bool m_err;
  LobbiesList m_lobbies;
  String m_nameStr;

  bool m_connected;
};

}