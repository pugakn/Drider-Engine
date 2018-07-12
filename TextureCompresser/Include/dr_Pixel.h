#pragma once
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

  /**
  * TEST::moveContructor
  *
  * Move constructor.
  *
  */
  Pixel(Pixel&& V) = default;

  /**
  * TEST::copyConstructor
  *
  * Copy constructor.
  *
  */
  Pixel(const Pixel& V);

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
  * TEST::defultDesstructor
  *
  *
  */
  ~Pixel();


  Pixel&
  operator=(const Pixel& A);
  
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