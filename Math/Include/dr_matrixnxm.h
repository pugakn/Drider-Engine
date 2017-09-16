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
  * Default constructor.
  *
  */
  MatrixNxM()
  {}

  MatrixNxM( std::initializer_list<VectorN<_cols>> _rows ) : m_data(rows)
  {
  }

  /**
  * Constructor using a scalar value.
  *
  *	@param _scalar
  *	 All the elements of the matrix are initialized to this value.
  *
  */
  explicit MatrixNxM(Float32 _scalar) : MatrixNxM(VectorN<_cols>( _scalar ))
  {}

  /**
  * Constructor using a vector.
  *
  *	@param _vec
  *	 All the rows of the matrix are initialized to _vec.
  *
  */
  explicit MatrixNxM(const VectorN<_cols>& _vec)
  {
	for(auto& row : m_data){
	  row = _vec;
	}
  }

  /**
  * Gets a reference to the specified row (Vector) from the matrix.
  *
  *	@param row
  *	 The index of the row.
  *
  * @return
  *	  A reference to the element at the [row] position.
  *
  * @throws out_of_range
  *	  If the row is greater than number of rows in the matrix.
  */

  FORCEINLINE VectorN<_cols>&
  operator[](SizeT row)
  {
	if (row >= _rows) {
	  throw std::out_of_range("Row out of range");
	}

	return m_data[row];
  }

  /**
  * Gets a const reference to the specified row (Vector) from the matrix.
  *
  *	@param row
  *	 The index of the row.
  *
  * @return
  *	  A const reference to the element at the [row] position.
  *
  * @throws out_of_range
  *	  If the row is greater than number of rows in the matrix.
  */

  FORCEINLINE const VectorN<_cols>&
  operator[](SizeT row) const
  {
	if (row >= _rows) {
	  throw std::out_of_range("Row out of range");
	}

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

  //Uncommented

  FORCEINLINE MatrixNxM<_cols, _rows>
  transpose() const
  {
	MatrixNxM<_cols, _rows> transposed;

	for(Int32 iRow = 0; iRow < _rows; ++iRow){
	  for(Int32 iCol = 0; iCol < _cols; ++iCol){
		transposed[iCol][iRow] = m_data[iRow][iCol];
	  }
	}

	return transposed;
  }

  //Uncommented

  FORCEINLINE MatrixNxM&
  operator*=(Float32 scalar)
  {	
	for(auto& row : m_data){
	  row *= scalar;	
	}

	return *this;
  }

  //Uncommented

  FORCEINLINE MatrixNxM
  operator*(Float32 scalar)
  {
	MatrixNxM temp(*this);
	temp *= scalar;
	return temp;
  }

  //Uncommented

  FORCEINLINE VectorN<_rows>
  operator*(const VectorN<_cols>& rhs)
  {
	VectorN<_rows> temp;
	for(Int32 iRow = 0; iRow < _rows; ++iRow){
	  temp[iRow] = rhs.dot(m_data[iRow]);	
	}

	return temp;
  }
  
 protected:

 private:
   
  std::array<VectorN<_cols>, _rows> m_data;
};

template<SizeT _rows, SizeT _cols> 
FORCEINLINE VectorN<_cols> 
operator*(const VectorN<_rows> lhs, const MatrixNxM<_rows, _cols> & rhs)
{
  VectorN<_cols> temp;
  MatrixNxM<_cols, _rows> rhsTransposed = rhs.transpose();
  for(Int32 iCol = 0; iCol < _cols; ++iCol){
	temp[iCol] = lhs.dot(rhsTransposed[iCol]);
  }
  return temp;
}


}