#include "DrawableAnimated.h"

namespace driderSDK {

DrawableAnimated::DrawableAnimated(Model* _model, GameObject& _gameObj)  
  : GameComponent(_gameObj,_T("asdfg")),
    m_model(_model)
{}

void 
DrawableAnimated::onCreate() {
  
  
}

void 
DrawableAnimated::onUpdate() {}

void 
DrawableAnimated::onRender() {}

void 
DrawableAnimated::onDestroy() {}
}