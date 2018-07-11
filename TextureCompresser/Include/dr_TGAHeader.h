#include <vector>
#include <dr_types.h>

namespace driderSDK {

class TGAHeader
{
 public:
  /*
  * TEST::defaultConstructor
  * 
  */
  TGAHeader();

  /*
  * TEST::defaultDestructor
  *
  */
  ~TGAHeader();

  /*
  * TEST::loadFromRawData
  *
  */
  void
  loadFromRawData(UInt8* data);

  /*
  * TEST::getRawData
  *
  */
  void
  getRawData(UInt8* data);

  //Length of the image ID field
  UInt8 ID_Length;
  //Whether a color map is included
  UInt8 ColorMapType;
  //Compression and color types
  UInt8 ImageType;

  //index of first color map entry that is included in the file
  UInt16 FirstEntryIndex;
  //number of entries of the color map that are included in the file
  UInt16 ColorMapLength;
  //number of bits per pixel
  UInt8 ColorMapEntrySize;

  //Absolute coordinate of lower-left corner for displays where origin is at the lower left
  UInt16 X_origin;
  //Absolute coordinate of lower-left corner for displays where origin is at the lower left
  UInt16 Y_origin;
  //Width in pixels
  UInt16 ImageWidth;
  //Height in pixels
  UInt16 ImageHeight;
  //Bits per pixel
  UInt8 PixelDepth;
  //Bits 3 - 0 give the alpha channel depth, bits 5 - 4 give direction
  UInt8 ImageDescriptor;
};

}