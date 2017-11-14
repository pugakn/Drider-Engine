#pragma once
#include "dr_core_prerequisites.h"
#define STB_IMAGE_IMPLEMENTATION
#include <STB\stb-master\stb_image.h>

namespace driderSDK {

struct DR_CORE_EXPORT Loader
{
 public:
  /**
  * TEST::texture
  * Loads texture
  *
  * @param name
  *   The name of the texture
  *
  * @param width
  *   Width returned
  *
  * @param height
  *   Height returned
  *
  * @param nrChannels
  *   Channels returned
  *
  * @return
  *   Buffer unsigned char* of the texture
  */
  static unsigned char*
  texture(TString name,
          Int32 *width,
          Int32 *height,
          Int32 *nrChannels);

  /**
  * TEST::texture
  * Free the memory of the texture
  *
  * @param data
  *   unsigned char* to free
  */
  static void
  freeTexture(unsigned char* data);
};

}