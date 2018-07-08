#include <vector>
#include <dr_types.h>

namespace driderSDK {

typedef union PixelInfo
{
  std::uint32_t color;
  struct
  {
    std::uint8_t b, g, r, a;
  };
};

class Tga
{
 public:

  /**
  * TEST::defaultConstructor
  *
  * Default class constructor.
  */
  Tga();

  /**
  * TEST::loadFileConstructor
  *
  * Initializer class constructor.
  *
  * @param FilePath
  *  Reference to the filepath of the image.
  */
  Tga(const char* FilePath);

  /**
  * TEST::GetPixels
  *
  * Description.
  *
  * @return
  *  Description.
  */
  std::vector<PixelInfo>
  GetPixels();

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

  bool m_ImageCompressed;

  std::vector<PixelInfo> m_Pixels;

  UInt32 m_width, m_height, m_numberOfPixels, m_size, m_BitsPerPixel;
};

}