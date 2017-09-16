#pragma once

#include <cmath>
#include <dr_prerequisites.h>

namespace driderSDK {

class Vector2D
{
 public:
  /**
  * Default constructor
  */

  Vector2D()
    : m_x(0.0f),
      m_y(0.0f)
  {};

  /**
  * Move constructor
  *
  */

  Vector2D(Vector2D && V) = default;

  /**
  * Copy constructor
  *
  */

  Vector2D(const Vector2D & V)
    : m_x(V.m_x),
      m_y(V.m_y)
  {};

  /**
  * Initialize constructor with values.
  *
  * @param _x
  * The x value of the vector
  *
  * @param _y
  * The y value of the vector
  */

  Vector2D(Float32 x, Float32 y)
    : m_x(x),
      m_y(y)
  {};

  /**
  * Default destructor
  *
  */

  ~Vector2D()
  {};

  /**
  * Computes the dot product of this vector and the vector parameter.
  * This operatios is commutative.
  *
  * @param B
  *  Right value of operation.
  *
  * @return
  *   The sum of the products of the corresponding entries of the
  *   vectors.
  */

  FORCEINLINE Float32
  dot(const Vector2D B) const
  {
  return (m_x*B.m_x) + (m_y*B.m_y);
  };

  /**
  * Computes the magnitude of this vector.
  *
  * @return
  *   The magnitude (or "size") of the vector.
  */

  FORCEINLINE Float32
  magnitude() const
  {
  return sqrt(dot(*this));
  };

  /**
  * Get this vector normalized.
  *
  * @return
  *   This vector normalized.
  */

  FORCEINLINE Vector2D
  normalize() const
  {
  return (*this) / magnitude();
  };

  FORCEINLINE Vector2D
  operator+ (const Vector2D & A) const
  {
  return Vector2D(m_x + A.m_x, m_y + A.m_y);
  };
  FORCEINLINE Vector2D&
  operator+= (const Vector2D & A)
  {
  m_x += A.m_x;
  m_y += A.m_y;
  return *this;
  };

  FORCEINLINE Vector2D
  operator- (const Vector2D & A) const
  {
  return Vector2D(m_x - A.m_x, m_y - A.m_y);
  };
  FORCEINLINE Vector2D&
  operator-= (const Vector2D & A)
  {
  m_x -= A.m_x;
  m_y -= A.m_y;
  return *this;
  };

  FORCEINLINE Vector2D
  operator* (const Vector2D & A) const
  {
  return Vector2D(m_x*A.m_x, m_y*A.m_y);
  };
  FORCEINLINE Vector2D&
  operator*= (const Vector2D & A)
  {
  m_x *= A.m_x;
  m_y *= A.m_y;
  return *this;
  };

  FORCEINLINE Vector2D
  operator* (const Float32 S) const
  {
    return Vector2D(m_x*S, m_y*S);
  };
  FORCEINLINE Vector2D&
  operator*= (const Float32 S)
  {
    m_x *= S;
    m_y *= S;
    return *this;
  };

  FORCEINLINE Vector2D
  operator/ (const Float32 S) const
  {
  return Vector2D(m_x, m_y);
  };
  FORCEINLINE Vector2D&
  operator/= (const Float32 S)
  {
  m_x;
  m_y;
  return *this;
  };

  union
  {
    struct
    {
      Float32 m_x, m_y;
    };
    Float32 v[2];
  };
};

}