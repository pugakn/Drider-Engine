#include <dr_types.h>

namespace driderSDK {

class Pixel
{
 public:
  /*
  * TEST::defultConstructor
  *
  * 
  */
  Pixel();

  /*
  * TEST::colorConstructor
  *
  *
  */
  Pixel(UInt8 red, UInt8 green, UInt8 blue);



  /*
  * TEST::colorWithAlphaConstructor
  *
  *
  */
  Pixel(UInt8 red, UInt8 green, UInt8 blue, UInt8 alpha);

  /*
  * TEST::defultConstructor
  *
  *
  */
  ~Pixel();
  
  union
  {
    UInt32 color;
    struct
    {
      UInt8 b, g, r, a;
    };
  };

  static Pixel Red;
  static Pixel Green;
  static Pixel Blue;
  static Pixel White;
  static Pixel Black;
  static Pixel Transparent;
  static Pixel Orange;
  static Pixel Celeste;
  static Pixel Purple;
  static Pixel Gray;
  static Pixel Brown;
  static Pixel Yellow;
  static Pixel Pink;
};

}