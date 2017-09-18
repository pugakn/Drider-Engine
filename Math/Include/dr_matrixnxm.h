#pragma once

#include <array>
#include <algorithm>
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
  *	 All the rows of the matrix are initialized to this vector.
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
  * Gets the transposed matrix.
  *
  *	@return
  *	  The matrix transposed.
  *
  */

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

  /**
  * Gets a constant pointer to the first element of the matrix.
  *
  * @return
  *	  A constant pointer to the first element of the matrix.
  */

  FORCEINLINE const Float32*
  ptr() const
  {
	return m_data[0].ptr();
  }

  /**
  *	Overload of binary operator *= 
  *
  *	@param scalar
  *	 The value 
  *
  */

  FORCEINLINE MatrixNxM&
  operator*=(Float32 scalar)
  {	
	for(auto& row : m_data){
	  row *= scalar;	
	}

	return *this;
  }  

  //Uncommented

  template<SizeT _rhsCols>
  FORCEINLINE MatrixNxM
  operator*=(const MatrixNxM<_cols, _rhsCols>& rhs)
  {
	MatrixNxM<_rows, _rhsCols> temp;
	MatrixNxM<_rhsCols, _cols> rhsTransposed = rhs.transpose();
	
	for(int iRows = 0; iRows < _rows; ++iRows){
	  temp[iRows] = m_data[iRows].dot(rhsTransposed[iRows]);
	}

	return temp;
  }

  //Uncommented

  FORCEINLINE MatrixNxM&
  operator/=(Float32 scalar)
  {	
	return *this *= (1.f / scalar);
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

  /**
  * Computes the equality operator row to row of both matrices.
  *
  * @param rhs
  *  The matrices with which the rows are compared.
  *
  *	@return 
  *	  True if all rows of *this vector are equal to all rows of rhs
  *	  vector, false otherwise.
  *
  */
  FORCEINLINE bool
  operator==(const MatrixNxM& rhs)
  {
	//Compares each element in the first range (m_data.begin()-m_data.end())
	//to each element in the second range(rhs.m_data.begin()-m_data.end()
	return std::equal(m_data.begin(), 
					  m_data.end(), 
					  rhs.m_data.begin(),
					  rhs.m_data.end());
  }

  /**
  *
  *
  */
  FORCEINLINE bool
  operator!=(const MatrixNxM& rhs)
  {
	return !(*this == rhs);
  }

 protected:

 private:
   
  std::array<VectorN<_cols>, _rows> m_data;
};

//Uncommented

template<SizeT _rows, SizeT _cols> 
FORCEINLINE VectorN<_cols> 
operator*(const VectorN<_rows> lhs, const MatrixNxM<_rows, _cols>& rhs)
{
  VectorN<_cols> temp;
  MatrixNxM<_cols, _rows> rhsTransposed = rhs.transpose();
  for(Int32 iCol = 0; iCol < _cols; ++iCol){
	temp[iCol] = lhs.dot(rhsTransposed[iCol]);
  }
  return temp;
}

//Uncommented

template<SizeT _rows, SizeT _cols>
FORCEINLINE MatrixNxM<_rows, _cols>
operator*(MatrixNxM<_rows, _cols> matrix, Float32 scalar)
{
  return matrix *= scalar;
}

//Uncommented

template<SizeT _rows, SizeT _cols>
FORCEINLINE MatrixNxM<_rows, _cols>
operator*(Float32 scalar, const MatrixNxM<_rows, _cols>& matrix)
{
  return matrix * scalar;
}

//Uncommented
template<SizeT _rows, SizeT _cols, SizeT _rhsCols>
FORCEINLINE MatrixNxM<_rows, _rhsCols>
operator*(const MatrixNxM<_rows, _cols>& lhs, const MatrixNxM<_cols, _rhsCols>& rhs)
{
  MatrixNxM<_rows, _rhsCols> temp;
  MatrixNxM<_rhsCols, _cols> rhsTransposed = rhs.transpose();
	
  for(int iRows = 0; iRows < _rows; ++iRows){
	for(int iCols = 0; iCols < _rhsCols; ++iCols){
	  temp[iRows][iCols] = lhs[iRows].dot(rhsTransposed[iCols]); 
	}
  }

  return temp;
}

//Uncommented

template<SizeT _rows, SizeT _cols>
FORCEINLINE MatrixNxM<_rows, _cols>
operator/(MatrixNxM<_rows, _cols> matrix, Float32 scalar)
{
  return matrix /= scalar;
}


}