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
  * Constructor using a scalar value
  *	
  *	@param _scalar
  *	 All the elements of the vector are initialized to this value.
  *
  */

  explicit VectorN( Float32 _scalar ) : m_data{ _scalar }
  {}

  /**
  * Number of elements in the vector
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
  * Gets a reference to the specified element from the vector
  *
  *	@param row
  *	 The index of the element.
  *
  * @return 
  *	  A reference to the element at the [index] position.
  */

  FORCEINLINE Float32& 
  operator[]( SizeT index )
  {
	if( index >= _elements )  
	{
	  throw std::out_of_range("Index out of range");
	}

	return m_data[index];
  }

  /**
  * Gets a const reference to the specified element from the vector
  *
  *	@param row
  *	 The index of the element.
  *
  * @return 
  *	  A const reference to the element at the [index] position.
  */

  FORCEINLINE const Float32& 
  operator[]( SizeT index ) const
  {
	if( index >= _elements )  
	{
	  throw std::out_of_range("Index out of range");
	}

	return m_data[index];
  }

  FORCEINLINE Float32
  dot( const VectorN<_elements>& other ) const
  {
	Float32 sum = 0;
	
	//For each element on the vector
	for( int iElement = 0; iElement < _elements; ++i )
	{
	  sum += m_data[iElement]
	}

  }

 protected:
 private:
   std::array<Float32, _elements> m_data;
};

}