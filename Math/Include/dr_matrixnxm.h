#pragma once

#include <array>
#include <dr_prerequisites.h>
#include "dr_vectorn.h"

namespace driderSDK {

/**
*  Matrix template with fixed rows and columns.
*
* Sample usage:
*	MatrixNxM<5,8> myMatrix;
*/

template<SizeT _rows, SizeT _cols>
class MatrixNxM
{
 public:
  /**
  * Default constructor
  *
  */
  MatrixNxM()
  {}

  /**
  * Constructor using a scalar value
  *	
  *	@param _scalar
  *	 All the elements of the matrix are initialized to this value.
  *
  */
  explicit MatrixNxM( Float32 _scalar ) : m_data{ VectorN<_cols>(_scalar) }
  {}

  /**
  * Constructor using a vector
  *	
  *	@param _vec
  *	 All the rows of the matrix are initialized to _vec.
  *
  */
  explicit MatrixNxM( const VectorN<_cols>& _vec ) : m_data{ _vec }
  {}

  /**
  * Gets a reference to the specified row (Vector) from the matrix
  *
  *	@param row
  *	 The index of the row.
  *
  * @return 
  *	  A reference to the element at the [row] position.
  */

  FORCEINLINE VectorN<_cols>& 
  operator[]( SizeT row )
  {
	//Change to throw aproach
	DR_ASSERT( row >= _rows );
	return m_data[row];
  }

  /**
  * Gets a const reference to the specified row (Vector) from the matrix
  *
  *	@param row
  *	 The index of the row.
  *
  * @return 
  *	  A const reference to the element at the [row] position.
  */

  FORCEINLINE const VectorN<_cols>& 
  operator[]( SizeT row ) const
  {
	//Change to throw aproach
	DR_ASSERT( row >= _rows );
	return m_data[row];
  }

  /**
  * Gets the number of rows in the matrix.
  * 
  *	@return
  *	  The number of rows in the matrix.
  */

  FORCEINLINE SizeT
  rows() const
  {
	return _rows;
  }

  /**
  * Gets the number of columns in the matrix.
  * 
  * @return 
  *	  The number of columns in the matrix.
  */

  FORCEINLINE SizeT
  cols() const noexcept
  {
	return _cols;
  }

  /**
  * Gets a constant pointer to the first element of the matrix.
  * 
  * @return 
  *	  A constant pointer to the first element of the matrix.
  */

  FORCEINLINE const Float32*
  ptr() const
  {
	return m_data[0][0];
  }

 protected:

 private:
  std::array<VectorN<_cols>, _rows> m_data;
};
}