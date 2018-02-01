#pragma once
#include "dr_core_prerequisites.h"
#include <vector>
#include "dr_vector3d.h"
namespace driderSDK {
  struct DR_CORE_EXPORT SplinePoint : public Vector3D {
  public:
    SplinePoint() : m_length(0), m_velocity(0) {}
    SplinePoint(float a, float b, float c) : m_length(0), m_velocity(0) {
      x = a;
      y = b;
      z = c;
    }
    float m_length;
    float m_velocity;
  };
  class Spline {
  public:
    const float STEP_SIZE = 0.005f;
    void 
    Init();
    void 
    ReCalculateSegmentsLength();
    explicit 
    Spline(bool _looped = false) : m_looped(_looped), m_totalLength(0) {}
    explicit 
    Spline(std::vector<SplinePoint>& points, bool _looped = false) :
      m_looped(_looped), m_points(points), m_totalLength(0) {}
    SplinePoint 
    GetPoint(float t);
    Vector3D 
    GetGradient(float t);
    float 
    GetSegmentLength(int node);
    float 
    GetNormalizedOffset(float p);

    std::vector<SplinePoint> m_points;
    float m_totalLength;
    bool m_looped;
  };

  class SplineAgent {
  private:
    float realLoc;
  public:
    SplineAgent() {
      realLoc = 0;
      m_acceleration = 0;
      m_velocity = 0;
      m_moving = false;
      m_pSpline = 0;
    }
    void 
    Update(float delta);
    void 
    SetOffset(float);
    Spline* m_pSpline;
    SplinePoint m_actualPoint;
    Vector3D m_actualGradient;

    float m_acceleration;
    bool m_moving;
    float m_velocity;
  };
}