#pragma once

#include <dr_vector3d.h>
#include <dr_matrix4x4.h>
#include "dr_core_prerequisites.h"

namespace driderSDK {

class Radian;
class Degree;

namespace AXIS {
  enum E {
    kX = 0,
    kY = 1,
    kZ = 2
  };
}

class DR_CORE_EXPORT Transform 
{
 public:
  Transform();

  /**
  * Computes the matrix containing the translation, orientation and
  * scale.
  *
  * @return 
  *   The transformation matrix.
  */
  const Matrix4x4& getTransformMatrix();
  const Vector3D& getPosition() const;

  /**
  * Gets the rotation vector containing the X, Y & Z angles in
  * radians.
  */
  const Vector3D& getRotation() const;
  const Vector3D& getScale() const;

  /**
  * Sets the value of the specified position component.
  * 
  * @param pos
  *  Value of the component.
  * 
  * @param axis
  *  The position component (x|y|z) to be modified
  */  
  void setPosition(float pos, AXIS::E axis);
  void setPosition(const Vector3D& position);


  void move(float dist, AXIS::E axis);
  void move(const Vector3D& distance);

  /**
  * Sets the value of the specified rotation component.
  * 
  * @param angle
  *  Angle of the component in radians.
  * 
  * @param axis
  *  The rotation component (x|y|z) to be modified
  */  
  void setRotation(Radian angle, AXIS::E axis);

  /**
  * Sets the value of the specified rotation component.
  * 
  * @param angle
  *  Angle of the component in degrees.
  * 
  * @param axis
  *  The rotation component (x|y|z) to be modified
  */  
  void setRotation(Degree angle, AXIS::E axis);

   /**
  * Sets the rotation vector of the transform.
  * 
  * @param orientation
  *  Vector with the rotation angles in radians.
  */ 
  void setRotation(const Vector3D& orientation);

  void rotate(Radian Radian, AXIS::E axis);
  void rotate(Degree Radian, AXIS::E axis);

  /**
  * Rotates the transform using a rotation vector.
  * 
  * @param rotation
  *  Vector with the rotation angles in radians.
  */ 
  void rotate(const Vector3D& rotation); 

  /**
  * Sets the value of the specified scale component.
  * 
  * @param scale
  *  The new scale of the component.
  * 
  * @param axis
  *  The scale component (x|y|z) to be setted.
  */  
  void setScale(float scale, AXIS::E axis);
  void setScale(const Vector3D& scale);

  /**
  * Scales the value of the specified scale component.
  * 
  * @param scale
  *  Scaling factor.
  * 
  * @param axis
  *  The scale component (x|y|z) to be scaled.
  */  
  void scale(float scale, AXIS::E axis);
  void scale(const Vector3D& scale);

 private:
  /**
  * Invalidates the transform matrix.
  * 
  * Used to specify whether the position, rotation or scale were modified 
  * and the transformation matrix needs to be updated.
  */
  void invalidate();

  /**
  * Computes the transformation matrix according to the position, 
  * rotation and scale established.
  */
  void update();

  bool m_outdatedTransform;
  Vector3D m_position;
  Vector3D m_rotation;
  Vector3D m_scale;
  Matrix4x4 m_transform;
};

}
