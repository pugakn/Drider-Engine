
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
  toggleSkeletonView(GameObject* obj);

  void
  toggleAABBDebug(GameObject* obj);

  void
  toggleWireframe();

  std::unique_ptr<Technique> m_animTech;
  std::unique_ptr<Technique> m_staticTech;
  std::unique_ptr<Technique> m_linesTech;

  bool m_drawMeshes;
  GameObject* m_left;
  GameObject* m_right;
  Timer m_timer;
};

}