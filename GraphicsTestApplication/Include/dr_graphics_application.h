
#include <dr_application.h>
#include <dr_memory.h>
#include <dr_util_prerequisites.h>
#include <dr_timer.h>

namespace driderSDK {

class GameObject;
class Model;
class Technique;

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

  void
  recompileShaders();

  void
  initModules();

  void
  initInputCallbacks();

  void 
  loadResources();

  void
  createTechniques();

  void
  createScene();

  std::shared_ptr<GameObject>
  addObjectFromModel(std::shared_ptr<Model> model,
                     const TString& name);

  void
  destroyModules();

  void
  printSceneHierachy(GameObject* obj, const TString& msg);

  void
  toggleSkeletonView(GameObject* obj);

  void
  toggleAABBDebug(GameObject* obj);

  void
  toggleWireframe();

  void 
  toggleCamera();

  void
  playerMovement();

  void
  playerRotation();

  std::unique_ptr<Technique> m_animTech;
  std::unique_ptr<Technique> m_staticTech;
  std::unique_ptr<Technique> m_linesTech;

  Int32 m_currCam;

  TString m_camNames[2];

  bool m_lockView;
  bool m_drawMeshes;
  GameObject* m_right;
  GameObject* m_player;
  GameObject* m_cameraHolder;
  Timer m_timer;
};

}