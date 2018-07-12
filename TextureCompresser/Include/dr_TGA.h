#pragma once
#include "dr_TGAHeader.h"
#include "dr_pixel.h"
#include <vector>

namespace driderSDK {

class TGA
{
 public:

  /**
  * TEST::defaultConstructor
  *
  * Default class constructor.
  */
  TGA();

  /**
  * TEST::loadFileConstructor
  *
  * Initializer class constructor.
  *
  * @param FilePath
  *  Reference to the filepath of the image.
  */
  TGA(TString FilePath);

  /**
  * TEST::load
  *
  * Loads a TGA.
  *
  * @param FilePath
  *  Reference to the filepath of the TGA image.
  */
  bool
  load(TString FilePath);

  /**
  * TEST::save
  *
  * Saves a TGA.
  *
  * @param FilePath
  *  Reference to the filepath where
  *  the TGA image will be saved.
  */
  void
  save(TString FilePath);

  /**
  * TEST::create
  *
  * Creates an uncompressed TGA.
  *
  * @param width
  *  Width of the texture.

  * @param height
  *  Height of the texture.
  */
  void
  create(SizeT width, SizeT height);

  /**
  * TEST::GetPixels
  *
  * Description.
  *
  * @return
  *  Description.
  */
  std::vector<Pixel>
  GetPixels();

  /**
  * TEST::GetPixel
  *
  * Description.
  *
  * @return
  *  Description.
  */
  Pixel&
  GetPixel(SizeT pixelIndex);

  /**
  * TEST::GetWidth
  *
  * Description.
  *
  * @return
  *  Description.
  */
  UInt32
  GetWidth() const;

  /**
  * TEST::GetHeight
  *
  * Description.
  *
  * @return
  *  Description.
  */
  UInt32
  GetHeight() const;

  /**
  * TEST::HasAlphaChannel
  *
  * Description.
  *
  * @return
  *  Description.
  */
  bool
  HasAlphaChannel();

 private:
  TGAHeader m_header;

  bool m_ImageCompressed;

  UInt32 m_width, m_height, m_numberOfPixels, m_size, m_BitsPerPixel;

  std::vector<Pixel> m_Pixels;

};

}