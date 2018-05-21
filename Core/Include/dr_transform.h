#pragma once

#include <dr_vector3d.h>
#include <dr_matrix4x4.h>
#include "dr_core_prerequisites.h"

#include <dr_export_script.h>
#include <..\..\Script\Include\dr_script_engine.h>

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

class Transform;
Transform* Ref_Transform();

class DR_CORE_EXPORT Transform 
{
 public:
  friend class GameObject;
  friend class BoneAttachObject;
  
  Int32 refCount;
  
  Transform();

  /**
  * Copy constructor
  */
  Transform(const Transform& other);

  /**
  * Computes the matrix containing the translation, orientation and
  * scale.
  *
  * @return 
  *   The transformation matrix.
  */
  const Matrix4x4& 
  getMatrix() const;

  const Vector3D& 
  getPosition() const;

  const Vector3D&
  getEulerAngles() const;

  
  /**
  * Gets the rotation matrix.
  */
  const Matrix4x4& 
  getRotation() const;

  const Vector3D& 
  getScale() const;

  /**
  * Gets the current facing direction.
  *
  * @return
  *   Vector with the direction of the transform.
  */
  const Vector3D&
  getDirection() const;

  /**
  * Sets the value of the specified position component.
  * 
  * @param pos
  *  Value of the component.
  * 
  * @param axis
  *  The position component (x|y|z) to be modified
  */  
  void 
  setPosition(float pos, AXIS::E axis);
  void 
  setPosition(const Vector3D& position);


  void 
  move(float dist, AXIS::E axis);
  void 
  move(const Vector3D& distance);

  /**
  * Sets the value of the specified rotation component.
  * 
  * @param angle
  *  Angle of the component in radians.
  * 
  * @param axis
  *  The rotation component (x|y|z) to be modified
  */  
  void 
  setRotation(Radian angle, AXIS::E axis);

  /**
  * Sets the value of the specified rotation component.
  * 
  * @param angle
  *  Angle of the component in degrees.
  * 
  * @param axis
  *  The rotation component (x|y|z) to be modified
  */  
  void 
  setRotation(Degree angle, AXIS::E axis);

   /**
  * Sets the rotation vector of the transform.
  * 
  * @param orientation
  *  Vector with the rotation angles in radians.
  */ 
  void 
  setRotation(const Vector3D& orientation);

  /**
  * Rotates the transform using a rotation vector.
  * 
  * @param rotation
  *  Vector with the rotation angles in radians.
  */ 
  void 
  rotate(const Vector3D& rotation); 

  /**
  * Sets the value of the specified scale component.
  * 
  * @param scale
  *  The new scale of the component.
  * 
  * @param axis
  *  The scale component (x|y|z) to be setted.
  */  
  void 
  setScale(float scale, AXIS::E axis);
  void 
  setScale(const Vector3D& scale);

  /**
  * Scales the value of the specified scale component.
  * 
  * @param scale
  *  Scaling factor.
  * 
  * @param axis
  *  The scale component (x|y|z) to be scaled.
  */  
  void 
  scale(float scale, AXIS::E axis);
  void 
  scale(const Vector3D& scale);

  Transform
  operator*(const Transform& other) const;

  Transform
  operator*(const Matrix4x4& mat) const;

  Transform&
  operator=(const Transform& other);
  
  void addRef() {
    refCount++;
  }

  void release() {
    if (--refCount == 0)
      delete this;
  }

  static BEGINING_REGISTER(Transform, 0, asOBJ_REF)

  result = scriptEngine->m_scriptEngine->RegisterObjectBehaviour("Transform",
                                                                 asBEHAVE_FACTORY,
                                                                 "Transform @f()",
                                                                 asFUNCTION(Ref_Transform),
                                                                 asCALL_CDECL);

  result = scriptEngine->m_scriptEngine->RegisterObjectBehaviour("Transform", 
                                                                 asBEHAVE_ADDREF, 
                                                                 "void f()", 
                                                                 asMETHOD(Transform, addRef), 
                                                                 asCALL_THISCALL);
  result = scriptEngine->m_scriptEngine->RegisterObjectBehaviour("Transform", 
                                                                 asBEHAVE_RELEASE, 
                                                                 "void f()", 
                                                                 asMETHOD(Transform, release), 
                                                                 asCALL_THISCALL);
  
  result = REGISTER_FOO_1P(Transform, move, const Vector3D&, void, "void", in)
  result = REGISTER_FOO_1P(Transform, rotate, const Vector3D&, void, "void", in)
  result = scriptEngine->m_scriptEngine->RegisterObjectProperty("Transform",
                                                                "Vector3D m_position",
                                                                asOFFSET(Transform, m_position));

   result = REGISTER_OP(Transform, operator=, opAssign, const Transform&, Transform&, "Transform&", in)

  END_REGISTER
  
 private:
  /**
  * Invalidates the transform matrix.
  * 
  * Used to specify whether the position, rotation or scale were modified 
  * and the transformation matrix needs to be updated.
  */
  void 
  invalidate();

  void 
  invalidateRotation();
  /**
  * Computes the transformation matrix according to the position, 
  * rotation and scale established.
  */
  void 
  update() const;
  
  bool
  changed() const;

  mutable Matrix4x4 m_transform;
  mutable Matrix4x4 m_rotation;
  mutable Vector3D m_direction;
  mutable Vector3D m_eulerAngles;
  mutable bool m_outdatedTransform;
  mutable bool m_outdatedRotation;
  bool m_change;
  Vector3D m_position;
  Matrix4x4 m_rotX;
  Matrix4x4 m_rotY;
  Matrix4x4 m_rotZ;
  Vector3D m_scale;
};

}
