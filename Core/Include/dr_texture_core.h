#pragma once

#include <functional>
#include <dr_memory.h>
#include "dr_resource.h"

namespace driderSDK {

class Texture;

class DR_CORE_EXPORT TextureCore : public Resource
{
public:

  TextureCore();

  ~TextureCore();
  
  TextureCore(const TextureCore&) = delete;

  TextureCore& operator=(const TextureCore&) = delete;

  virtual void
  init(void* data);

  using GFXDeleter = std::function<void(Texture*)>;
  using TexturePtr = std::unique_ptr<Texture, GFXDeleter>;

  TexturePtr textureGFX;
};

}