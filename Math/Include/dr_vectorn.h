#pragma once

#include <array>
#include <algorithm>
#include <dr_prerequisites.h>
#include "dr_math.h"

namespace driderSDK {

/**
*  Vector template with fixed number of elements.
*
* Sample usage:
*	VectorN<8> myVectorWithEightElements;
*/
template <SizeT _elements>
class VectorN
{
 public:
  /**
  * Default constructor
  */
  VectorN()
  {}

  /**
  * Constructor using a scalar value.
  *
  *	@param _scalar
  *	 All the elements of the vector are initialized to this value.
  *
  */
  explicit VectorN(float _scalar)
  {
	  for(auto& element : m_data){
	    element = _scalar;
	  }
  }
  
  /**
  * Number of elements in the vector.
  *
  *	@return
  *   The number of elements the vector contains.
  *
  */
  FORCEINLINE SizeT
  elements() const
  {
	  return _elements;
  }

  /**
  * Gets a reference to the specified element from the vector.
  *
  *	@param index
  *	 The index of the element.
  *
  * @return
  *	  A reference to the element at the [index] position.
  *
  * @throws out_of_range
  *	  If the index is greater than number of elements in the vector.
  */
  FORCEINLINE float&
  operator[](SizeT index)
  {
    DR_ASSERT(index < elements);
	  
	  return m_data[index];
  }

  /**
  * Gets a const reference to the specified element from the vector.
  *
  *	@param index
  *	 The index of the element.
  *
  * @return
  *	  A const reference to the element at the [index] position.
  *
  * @throws out_of_range
  *	  If the index is greater than number of elements in the vector.
  */
  FORCEINLINE const float&
  operator[](SizeT index) const
  {
	  DR_ASSERT(index < elements);


	  return m_data[index];
  }

  /**
  * Computes the dot product between 2 vectors.
  *
  *	@param other
  *	 The vector against which the dot product is calculated.
  *
  * @return
  *	  A const reference to the element at the [index] position.
  */
  FORCEINLINE float
  dot(const VectorN& other) const
  {
	  float sum = 0;

	  //For each element on the vector
	  for (Int32 iElement = 0; iElement < _elements; ++iElement) {
	    sum += m_data[iElement] * other.m_data[iElement];
	  }

	  return sum;
  }

  /**
  * Computes the length of the vector.
  *
  * @return
  *	  Length of the vector.
  */
  FORCEINLINE float
  length() const
  {
	  return sqrt(dot(*this));
  }

  /**
  * Computes the squared length of the vector.
  *
  * @return
  *	  Squared length of the vector.
  */
  FORCEINLINE float
  lengthSqr() const
  {
	  return dot(*this);
  }

  /**
  * Gets the vector normalized.
  *
  * @return
  *	  The vector normalized.
  *
  */
  FORCEINLINE VectorN
  normalize() const
  {
	  float invLength = 1.f / length();

	  DR_ASSERT(invLength);

	  return (*this) * invLength;
  }

   /**
  * Gets a constant pointer to the first element of the vector.
  *
  * @return
  *	  A constant pointer to the first element of the vector.
  */
  FORCEINLINE const float*
  ptr() const
  {
	  return &m_data[0];
  }

  /**
  *	Overload of binary operator *=
  *
  *	This operator performs a memberwise multiplication by a scalar
  * and assigns the result to *this.
  *
  *	@param scalar
  *	 Right operand (a scalar value).
  *
  * @return
  *	  A reference to the transformed vector (*this).
  *
  */
  FORCEINLINE VectorN&
  operator*=(float scalar)
  {
	  //For each element on the vector do the multiplication assigment
	  for(auto& element : m_data){
	    element *= scalar;
	  }

    return *this;
  }

  /**
  *	Overload of binary operator *=
  *
  *	This operator performs a memberwise division by a scalar
  * and assigns the result to *this.
  *
  *	@param scalar
  *	 Right operand (a scalar value).
  *
  * @return
  *	  A reference to the transformed vector (*this).
  *
  */
  FORCEINLINE VectorN&
  operator/=(float scalar)
  {
    //Multiply by reciprocal instead of multiple divisions
	  float invScalar = 1.f / scalar;
	  (*this) *= invScalar;
	  return (*this);
  }

  /**
  *	Overload of binary operator +=.
  * 
  *	This operator performs a memberwise addition of both vectors
  * and assigns the result to *this.
  *
  *	@param rhs
  *	 Right operand (a vector with same number of elements).
  *
  * @return
  *	  A reference to the transformed vector (*this).
  *
  */
  FORCEINLINE VectorN&
  operator+=(const VectorN& rhs)
  {
	  //For each element on the vector do the addition assigment
	  for (Int32 iElement = 0; iElement < _elements; ++i) {
	    m_data[iElement] += other.m_data[iElement];
	  }
  }

  /**
  *	Overload of binary operator +=.
  * 
  *	This operator performs a memberwise subtraction of both vectors
  * and assigns the result to *this.
  *
  *	@param rhs
  *	 Right operand (a vector with same number of elements).
  *
  * @return
  *	  A reference to the transformed vector (*this).
  *
  */
  FORCEINLINE VectorN&
  operator-=(const VectorN& rhs)
  {
	  //For each element on the vector do the subtraction assigment
	  for (Int32 iElement = 0; iElement < _elements; ++i) {
	    m_data[iElement] -= other.m_data[iElement];
	  }
  }

  /**
  * Overload of binary operator ==.
  *
  * This operator performs a memberwise equality comparison.
  *
  * @param rhs
  *  Right operand (a vector with same number of elements).
  *
  *	@return 
  *	  True if all elements of *this vector are equal to all elements of rhs
  *	  vector, false otherwise.
  *
  */
  FORCEINLINE bool
  operator==(const VectorN& rhs)
  {
	  //Compares each element in the first range (m_data.begin()-m_data.end())
	  //to each element in the second range(rhs.m_data.begin()-m_data.end()
	  return std::equal(m_data.begin(), 
					    m_data.end(), 
					    rhs.m_data.begin(), 
					    rhs.m_data.begin());
  }
	
  /**
  * Overload of binary operator ==.
  *
  * This operator performs a memberwise inequality comparison.
  *
  *	@return 
  *	  True if an elements of *this vector is unequal to an elements of rhs
  *	  vector, false otherwise.
  *
  */
  FORCEINLINE bool
  operator!=(const VectorN& rhs)
  {
	  return !(*this == rhs);
  }

 protected:

 private:
  std::array<float, _elements> m_data;
};

/**
* Overload of binary operator *.
*
* This operator performs a multiplication of each member of vector by scalar.
*
* @param vector
*  Left operand (a vector).
*
* @param scalar
*  Right operand (a scalar).
*
* @return
*	Memberwise multiplication by scalar.
*
*/
template<SizeT _elements>
static FORCEINLINE VectorN<_elements>
operator*(VectorN<_elements> vector, float scalar)
{
  return vector *= scalar;
}

/**
* Overload of binary operator *.
*
* This operator performs a multiplication of each member of vector by scalar.
*
* @param scalar
*	Left operand (a scalar).
*
* @param vector
*  Right operand (a vector).
*
* @return
*	Memberwise multiplication by scalar.
*
*/
template<SizeT _elements>
static FORCEINLINE VectorN<_elements>
operator*(float scalar, const VectorN<_elements>& vector)
{
  return vector * scalar;
}

/**
* Overload of binary operator /.
*
* This operator performs a division of each member of vector by scalar.
*
* @param vector
*	Left operand (a vector).
*
* @param scalar
*  Right operand (a scalar).
*
* @return
*	Memberwise division by scalar.
*
*/
template<SizeT _elements>
static FORCEINLINE VectorN<_elements>
operator/(VectorN<_elements> vector, float scalar)
{
  return vector /= scalar;
}

/**
* Overload of binary operator +.
*
* This operator performs a memberwise addition of both vectors.
*
* @param lhs
*	Left operand (a vector).
*
* @param rhs
*  Right operand (a scalar).
*
* @return
*	Memberwise addition of both vectors.
*
*/
template<SizeT _elements>
static FORCEINLINE VectorN<_elements>
operator+(VectorN<_elements> lhs, const VectorN<_elements>& rhs)
{
  return lhs += rhs;
}

/**
* Overload of binary operator -.
*
* This operator performs a memberwise subtraction of both vectors.
*
* @param lhs
*	Left operand (a vector).
*
* @param rhs
*  Right operand (a scalar).
*
* @return
*	Memberwise subtraction of both vectors.
*
*/
template<SizeT _elements>
static FORCEINLINE VectorN<_elements>
operator-(VectorN<_elements> lhs, const VectorN<_elements>& rhs )
{
  return lhs -= rhs;
}

}