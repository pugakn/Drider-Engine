#include "dr_spline.h"
namespace driderSDK {
void 
Spline::Init()
{
  ReCalculateSegmentsLength();
}
void 
Spline::ReCalculateSegmentsLength()
{
  DR_ASSERT(m_points.size() >= 4);
  for (int i = 0; i < (m_points.size() - 3); i++)
  {
    m_totalLength += (m_points[i].m_length = GetSegmentLength(i));
  }
}
SplinePoint 
Spline::GetPoint(float t)
{
  int p0, p1, p2, p3;
  SplinePoint ret;
  if (!m_looped)
  {
    p1 = (int)t + 1;
    p2 = p1 + 1;
    p3 = p2 + 1;
    p0 = p1 - 1;
  }
  else
  {
    p1 = (int)t;
    p2 = (p1 + 1) % m_points.size();
    p3 = (p2 + 1) % m_points.size();
    p0 = p1 >= 1 ? p1 - 1 : m_points.size() - 1;
  }

  t = t - (int)t;
  if (t == 0) return m_points[p0];
  float tt = t * t;
  float ttt = tt * t;


  float q1 = -ttt + 2.0f*tt - t;
  float q2 = 3.0f*ttt - 5.0f*tt + 2.0f;
  float q3 = -3.0f*ttt + 4.0f*tt + t;
  float q4 = ttt - tt;


  ret.x = 0.5f * (m_points[p0].x * q1 + m_points[p1].x * q2 + m_points[p2].x * q3 + m_points[p3].x * q4);
  ret.y = 0.5f * (m_points[p0].y * q1 + m_points[p1].y * q2 + m_points[p2].y * q3 + m_points[p3].y * q4);
  ret.z = 0.5f * (m_points[p0].z * q1 + m_points[p1].z * q2 + m_points[p2].z * q3 + m_points[p3].z * q4);

  /*VELOCITY*/
  ret.m_velocity = m_points[p1].m_velocity + ((m_points[p2].m_velocity - m_points[p1].m_velocity)*t);
  return ret;
}
Vector3D 
Spline::GetGradient(float t)
{
  int p0, p1, p2, p3;
  SplinePoint ret;
  if (!m_looped)
  {
    p1 = (int)t + 1;
    p2 = p1 + 1;
    p3 = p2 + 1;
    p0 = p1 - 1;
  }
  else
  {
    p1 = (int)t;
    p2 = (p1 + 1) % m_points.size();
    p3 = (p2 + 1) % m_points.size();
    p0 = p1 >= 1 ? p1 - 1 : m_points.size() - 1;
  }

  t = t - (int)t;
  if (t == 0) return m_points[p0];

  float tt = t * t;
  float ttt = tt * t;

  float q1 = -3.0f * tt + 4.0f*t - 1;
  float q2 = 9.0f*tt - 10.0f*t;
  float q3 = -9.0f*tt + 8.0f*t + 1.0f;
  float q4 = 3.0f*tt - 2.0f*t;

  ret.x = 0.5f * (m_points[p0].x * q1 + m_points[p1].x * q2 + m_points[p2].x * q3 + m_points[p3].x * q4);
  ret.y = 0.5f * (m_points[p0].y * q1 + m_points[p1].y * q2 + m_points[p2].y * q3 + m_points[p3].y * q4);
  ret.z = 0.5f * (m_points[p0].z * q1 + m_points[p1].z * q2 + m_points[p2].z * q3 + m_points[p3].z * q4);

  return ret;
}
float 
Spline::GetSegmentLength(int node)
{
  float fLength = 0.0f;

  SplinePoint oldP, newP;
  oldP = GetPoint((float)node);

  for (float t = 0; t < 1.0f; t += STEP_SIZE)
  {
    newP = GetPoint((float)node + t);
    fLength += sqrtf((newP.x - oldP.x)*(newP.x - oldP.x) +
      (newP.y - oldP.y)*(newP.y - oldP.y) +
      (newP.z - oldP.z)*(newP.z - oldP.z));
    oldP = newP;
  }
  return fLength;
}
float 
Spline::GetNormalizedOffset(float p)
{
  int i = 0;
  while (p > m_points[i].m_length)
  {
    p -= m_points[i].m_length;
    i++;
  }
  return (float)i + (p / m_points[i].m_length);

}
void 
SplineAgent::Update(float delta)
{
  realLoc += delta * m_velocity;
  while (realLoc > m_pSpline->m_totalLength)
    realLoc -= m_pSpline->m_totalLength;
  while (realLoc < 0.0f)
    realLoc += m_pSpline->m_totalLength;
  m_actualPoint = m_pSpline->GetPoint(m_pSpline->GetNormalizedOffset(realLoc));
  m_actualGradient = m_pSpline->GetGradient(m_pSpline->GetNormalizedOffset(realLoc));
  m_actualGradient.normalize();
  m_velocity = m_actualPoint.m_velocity;
}
void 
SplineAgent::SetOffset(float offset)
{
  realLoc = offset;
}
}