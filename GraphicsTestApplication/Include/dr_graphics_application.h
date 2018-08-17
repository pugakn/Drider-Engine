
#include <atomic>
#include <array>
#include <map>
#include <vector>
#include <thread>

#include <dr_application.h>
#include <dr_camera.h>
#include <dr_light.h>
#include <dr_memory.h>
#include <dr_util_prerequisites.h>
#include <dr_timer.h>
#include <dr_renderman.h>
#include "PathHolder.h"
#include "PathRenderer.h"
#include "Spawner.h"

namespace driderSDK {

class GameObject;
class Model;
class Technique;

class RenderManagerT
{
public:

  RenderManagerT();
  
  ~RenderManagerT();
  
  void 
  renderLoop();

  void 
  render();

  void 
  init();

  void 
  close();
private:
  
  using TechniquePtr = std::unique_ptr<Technique, void(*)(Technique*)>;

  TechniquePtr m_animTech;
  TechniquePtr m_staticTech;
  TechniquePtr m_linesTech;
  std::atomic_bool m_run;
  Camera m_cam;
};

class GraphicsApplication : public Application
{
public:

  GraphicsApplication();

  ~GraphicsApplication();

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
    
  virtual void 
  onResize() override;

  void
  initModules();

  void
  initInputCallbacks();

  void 
  loadResources();
  
  void
  createScene();

  std::shared_ptr<GameObject>
  addObjectFromModel(std::shared_ptr<Model> model,
                     const TString& name);

  std::shared_ptr<GameObject>
  addObject(const TString& name, const TString& model, bool animated);

  void
  destroyModules();

  void
  printSceneHierachy(GameObject* obj, const TString& msg);

  void
  toggleSkeletonView(GameObject* obj);

  void
  toggleAABBDebug(std::shared_ptr<GameObject> obj);

  void
  toggleWireframe();

  void
  toggleAnimation();

  void 
  speedAnim(float val);

  void 
  toggleCamera();

  void
  initScriptEngine();

  void
  playerMovement();

  void
  playerRotation();

  std::vector<Vector3D>
  calculatePoints();
  
  Int32 m_currCam;
  Int32 m_currAnim;
  TString m_camNames[2];
  TString m_animationsNames[4];
  TString m_playerAnims[4];

  std::map<TString, TString> m_anims;

  bool m_lockView;
  bool m_drawMeshes;
  GameObject* m_right;
  GameObject* m_center;
  GameObject* m_player;
  GameObject* m_cameraHolder;
  Timer m_timer;
  //RenderMan m_renderMan;
  //std::array<Light, 128> m_light;
  std::vector<PathHolder> m_paths;
  std::vector<PathRenderer> m_pathRenders;
  Spawner m_spiderSpawn;
  // Inherited via Application
  RenderManagerT m_render;
  std::thread m_thread;
};

}