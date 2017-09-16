#pragma once

#include <array>
#include <dr_prerequisites.h>

namespace driderSDK {

template <SizeT _elements>
class VectorN
{
public:
  /**
  * Default constructor
  *
  */

  VectorN() : m_data{}
  {}

  /**
  * Constructor using a scalar value.
  *
  *	@param _scalar
  *	 All the elements of the vector are initialized to this value.
  *
  */

  explicit VectorN(Float32 _scalar)
  {
	for(auto& element : m_data){
	  element = _scalar;
	}
  }

  VectorN( std::initializer_list<Float32> _values ) : m_data(_values)
  {}

  /**
  * Number of elements in the vector.
  *
  *	@return
  *	 The number of elements the vector contains.
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
  *	@param row
  *	 The index of the element.
  *
  * @return
  *	  A reference to the element at the [index] position.
  *
  * @throws out_of_range
  *	  If the index is greater than number of elements in the vector.
  */

  FORCEINLINE Float32&
  operator[](SizeT index)
  {
	if (index >= _elements) {
	  throw std::out_of_range("Index out of range");
	}

	return m_data[index];
  }

  /**
  * Gets a const reference to the specified element from the vector.
  *
  *	@param row
  *	 The index of the element.
  *
  * @return
  *	  A const reference to the element at the [index] position.
  *
  * @throws out_of_range
  *	  If the index is greater than number of elements in the vector.
  */

  FORCEINLINE const Float32&
  operator[](SizeT index) const
  {
	if (index >= _elements) {
	  throw std::out_of_range("Index out of range");
	}

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

  FORCEINLINE Float32
  dot(const VectorN& other) const
  {
	Float32 sum = 0;

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

  FORCEINLINE Float32
  length() const
  {
	return sqrt(dot(*this));
  }

  //Uncommented

  FORCEINLINE VectorN
  normalize() const
  {
	Float32 invLength = 1.f / length();
	return (*this) * invLength;
  }

  //Uncommented

  FORCEINLINE VectorN&
  operator*=(Float32 scalar)
  {
	for(auto& element : m_data){
	  element *= scalar;
	}

	return *this;
  }

  //Uncommented
  FORCEINLINE VectorN&
  operator/=(Float32 scalar)
  {
	Float32 invScalar = 1.f / scalar;
	(*this) *= invScalar;
	return (*this);
  }
  //Uncommented

  FORCEINLINE VectorN&
  operator+=(const VectorN& rhs)
  {
	//For each element on the vector
	for (Int32 iElement = 0; iElement < _elements; ++i) {
	  m_data[iElement] += other.m_data[iElement];
	}
  }

  //Uncommented
  FORCEINLINE VectorN&
  operator-=(const VectorN& rhs)
  {
	//For each element on the vector
	for (Int32 iElement = 0; iElement < _elements; ++i) {
	  m_data[iElement] -= other.m_data[iElement];
	}
  }
    
protected:

private:
  std::array<Float32, _elements> m_data;
};

//Uncommented
template<SizeT _elements>
FORCEINLINE VectorN<_elements>
operator*(VectorN<_elements> vector, Float32 scalar)
{
  vector *= scalar;
  return vector;
}

//Uncommented
template<SizeT _elements>
FORCEINLINE VectorN<_elements>
operator*(Float32 scalar, const VectorN<_elements>& vector)
{
  return vector * scalar;
}

//Uncommented
template<SizeT _elements>
FORCEINLINE VectorN<_elements>
operator/(VectorN<_elements> vector, Float32 scalar)
{
  vector /= scalar;
  return temp;
}

//Uncommented
template<SizeT _elements>
FORCEINLINE VectorN<_elements>
operator+(VectorN<_elements> lhs, const VectorN<_elements>& rhs)
{
  lhs += rhs;
  return temp;
}

//Uncommented
template<SizeT _elements>
FORCEINLINE VectorN<_elements>
operator-(VectorN<_elements> lhs, const VectorN<_elements>& rhs )
{
  lhs -= rhs;
  return temp;
}

}