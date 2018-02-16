#include "dr_time.h"

namespace driderSDK {

Time::Time(Microseconds microseconds) 
  : m_duration(microseconds)
{}

Time::Time(Milliseconds milliseconds) {
  fromMilliseconds(milliseconds);
}

Time::Time(Seconds seconds) {
  fromSeconds(seconds);
}

Time::Time() : m_duration(0)
{}

float 
Time::asSeconds() const {
  using namespace std::chrono;
  return duration_cast<duration<Seconds>>(m_duration).count();
}

Int32 
Time::asMilliseconds() const {
  using namespace std::chrono;
  Int32 v = static_cast<Int32>(duration_cast<milliseconds>(m_duration).count());
  return v;
}

Int64 
Time::asMicroseconds() const {
  return m_duration.count();
}

void 
Time::fromSeconds(float seconds) {
  constexpr Int32 secsToMicro = 1000000;
  m_duration = Duration(static_cast<Int64>(seconds * secsToMicro));
}

void 
Time::fromMilliseconds(Int32 milliseconds) {
  constexpr Int32 milliToMicro = 1000;
  m_duration = Duration(milliseconds * milliToMicro);
}

void 
Time::fromMicroseconds(Int64 microseconds) {
  m_duration = Duration(microseconds);
}

bool 
Time::operator<(const Time & rhs) {
  return m_duration < rhs.m_duration;
}

bool 
Time::operator>(const Time & rhs) {
  return m_duration > rhs.m_duration;
}

bool 
Time::operator>=(const Time & rhs) {
  return !(*this < rhs);
}

bool 
Time::operator<=(const Time & rhs) {
  return !(*this > rhs);
}

bool 
Time::operator==(const Time& rhs) {
  return m_duration == rhs.m_duration;
}

bool Time::operator!=(const Time& rhs) {
  return !operator==(rhs);
}

Time& 
Time::operator%=(const Time& rhs) {
  m_duration %= rhs.m_duration;
  return *this;
}

Time& Time::operator+=(const Time& rhs) {
  m_duration += rhs.m_duration;
  return *this;
}

Time& Time::operator-=(const Time& rhs) {
  m_duration -= rhs.m_duration;
  return *this;
}

Time 
Time::operator%(Time rhs) {
  return Time(Microseconds(m_duration.count())) %= rhs;
}

Time 
Time::operator+(Time rhs) {
  return Time(Microseconds(m_duration.count())) += rhs;
}

Time 
Time::operator-(Time rhs) {
  return Time(Microseconds(m_duration.count())) -= rhs;
}

}