#pragma once

#include <array>
#include <algorithm>
#include "dr_math_prerequisites.h"
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
  * TEST::constructorOneVector
  * Default constructor.
  *
  */
  MatrixNxM()
  {}

  /**
  * TEST::constructorOneFloat
  * Constructor using a scalar value.
  *
  *	@param _scalar
  *	 All the elements of the matrix are initialized to this value.
  *
  */
  explicit MatrixNxM(float _scalar) : MatrixNxM(VectorN<_cols>( _scalar ))
  {}

  /**
  * TEST::constructorOneVector
  * Constructor using a vector.
  *
  *	@param _vec
  *	 All the rows of the matrix are initialized to this vector.
  *
  */
  explicit MatrixNxM(const VectorN<_cols>& _vec)
  {
    for (auto& row : m_elements){
	    row = _vec;
    }
  }
  
  /**
  * TEST::operatorBrackets
  * Gets a reference to the specified row (Vector) from the matrix.
  *
  *	@param row
  *	 The index of the row.
  *
  * @return
  *	  A reference to the element at the [row] position.
  */
  FORCEINLINE VectorN<_cols>&
  operator[](SizeT row)
  {
	  DR_ASSERT(row < _rows);

	  return m_elements[row];
  }

  /**
  * TEST::operatorBrackets
  * Gets a const reference to the specified row (Vector) from the matrix.
  *
  *	@param row
  *	 The index of the row.
  *
  * @return
  *	  A const reference to the element at the [row] position.
  */
  FORCEINLINE const VectorN<_cols>&
  operator[](SizeT row) const
  {
		DR_ASSERT(row < _rows);

	  return m_elements[row];
  }

  /**
  * TEST::rows
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
  * TEST::cols
  * Gets the number of columns in the matrix.
  *
  * @return
  *	  The number of columns in the matrix.
  */
  FORCEINLINE SizeT
  cols() const
  {
	  return _cols;
  }

  /**
  * TEST::transpose
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

	  for (Int32 iRow = 0; iRow < _rows; ++iRow) {
	    for (Int32 iCol = 0; iCol < _cols; ++iCol) {
		    transposed[iCol][iRow] = m_elements[iRow][iCol];
	    }
	  }

	  return transposed;
  }

  /**
  * TEST::ptr
  * Gets a pointer to the first element of the matrix.
  *
  * @return
  *	  A pointer to the first element of the matrix.
  */
  FORCEINLINE float*
  ptr() 
  {
    return m_elements[0].ptr();
  }

  /**
  * TEST::ptr
  * Gets a constant pointer to the first element of the matrix.
  *
  * @return
  *	  A constant pointer to the first element of the matrix.
  */
  FORCEINLINE const float*
  ptr() const
  {
	  return m_elements[0].ptr();
  }

  /**
  * TEST::assignByMultiplicationFloat
  *	Overload of binary operator *=.
  *
  *	This overload performs the multiplication assignment by scalar 
  *	to each row of the matrix.
  *
  *	@param scalar
  *	 Right side operand (a scalar)
  *
  *	@return
  *	  A reference to (*this).	
  */
  FORCEINLINE MatrixNxM&
  operator*=(float scalar)
  {	
	  for (auto& row : m_elements){
	    row *= scalar;	
	  }

	  return *this;
  }  

  /**
  * TEST::assignByDivisionFloat
  *	Overload of binary operator /=.
  *
  *	This overload performs the division assignment by scalar 
  *	to each row of the matrix.
  *
  *	@param scalar
  *	 Right side operand (a scalar)
  *
  *	@return
  *	  A reference to (*this).	
  */
  FORCEINLINE MatrixNxM&
  operator/=(float scalar)
  {	
	  return *this *= (1.f / scalar);
  }

  /**
  * TEST::operatorEquality
  *	Overload of binary operator ==.
  *
  *	This overload performs a memberwise equality comparision. 
  *
  * @param rhs
  *  Right operand (a matrice of the same size with which the rows 
  *	 are compared).
  *
  *	@return 
  *	  True if all rows of *this matrix are equal to all rows of rhs
  *	  matrix, false otherwise.
  *
  */
  FORCEINLINE bool
  operator==(const MatrixNxM& rhs)
  {
	  //Compares each element in the first range (m_elements.begin()-m_elements.end())
	  //to each element in the second range(rhs.m_elements.begin()-m_elements.end()
	  return std::equal(m_elements.begin(), 
					    m_elements.end(), 
					    rhs.m_elements.begin(),
					    rhs.m_elements.end());
  }

  /**
  * TEST::operatorInequality
  *	Overload of binary operator ==.
  *
  *	This overload performs a memberwise inequality comparision. 
  *
  * @param rhs
  *  Right operand (a matrice of the same size with which the rows 
  *	 are compared).
  *
  *	@return 
  *	  True if a row of *this matrix is unequal to a row of rhs
  *	  matrix, false otherwise.
  *
  */
  FORCEINLINE bool
  operator!=(const MatrixNxM& rhs)
  {
	  return !(*this == rhs);
  }

 protected:

 private:
   
  std::array<VectorN<_cols>, _rows> m_elements;
};

/**
* TEST::operatorMultiplicationVectorLeft
* Overload of binary operator *.
*
* This overload performs the vector-matrix product.
*
* @param lhs
*  Left side operand (a vector with N elements).
*	
* @param rhs
*  Right side operand (a matrix with N rows and M columns).
*
* @return
*	The transformed vector.	
*/
template<SizeT _rows, SizeT _cols> 
static FORCEINLINE VectorN<_cols> 
operator*(const VectorN<_rows> lhs, const MatrixNxM<_rows, _cols>& rhs)
{
  VectorN<_cols> temp;
  MatrixNxM<_cols, _rows> rhsTransposed = rhs.transpose();
  for (Int32 iCol = 0; iCol < _cols; ++iCol){
	  temp[iCol] = lhs.dot(rhsTransposed[iCol]);
  }
  return temp;
}

/**
* TEST::operatorMultiplicationVectorRight
*	Overload of binary operator *.
*
*	This overload performs the matrix-vector product.
*	
* @param lhs
*  Left side operand (a matrix)
*
*	@param rhs
*	 Right side operand (a vector with _cols number of elements)
*	 elements.
*
*	@return
*	  The transformed vector.	
*/
template<SizeT _rows, SizeT _cols> 
static FORCEINLINE VectorN<_rows> 
operator*(const MatrixNxM<_rows, _cols>& lhs, const VectorN<_cols> rhs)
{
    VectorN<_rows> temp;
	  for (Int32 iRow = 0; iRow < _rows; ++iRow){
	    temp[iRow] = rhs.dot(lhs[iRow]);	
	  }

	  return temp;
}

/**
* TEST::opeartorMultiplicationScalarRight
* Overload of binary operator *.
*
* This overload performs the multiplication of each row of matrix
* by scalar.
*	
* @param matrix
*  Left side operand (a matrix with N rows and M columns).
*
* @param scalar
*  Right side operand (a scalar).
*
* @return
*	The transformed matrix.	
*/
template<SizeT _rows, SizeT _cols>
static FORCEINLINE MatrixNxM<_rows, _cols>
operator*(MatrixNxM<_rows, _cols> matrix, float scalar)
{
  return matrix *= scalar;
}

/**
* TEST::opeartorMultiplicationScalarLeft
* Overload of binary operator *.
*
* This overload performs the multiplication of each row of matrix
* by scalar.
*	
* @param scalar
*  Left side operand (a scalar).
*
* @param matrix
*  Right side operand (a matrix with N rows and M columns).
*
* @return
*	The transformed matrix.	
*/
template<SizeT _rows, SizeT _cols>
static FORCEINLINE MatrixNxM<_rows, _cols>
operator*(float scalar, const MatrixNxM<_rows, _cols>& matrix)
{
  return matrix * scalar;
}

/**
* TEST::operatorDivisionScalar
* Overload of binary operator /.
*
* This overload performs the division of each row of matrix
* by scalar.
*	
* @param matrix
*  Left side operand (a matrix with N rows and M columns).
*
* @param scalar
*  Right side operand (a scalar).
*
* @return
*	The transformed matrix.	
*/
template<SizeT _rows, SizeT _cols>
static FORCEINLINE MatrixNxM<_rows, _cols>
operator/(MatrixNxM<_rows, _cols> matrix, float scalar)
{
  return matrix /= scalar;
}

/**
* TEST::operatorMultiplicationMatrix
* Overload of binary operator *.
* 
* This overload performs the matrices product.
* 
* @param lhs
*  Left size operand (a matrix of N rows x M columns).
* 
* @param rhs
*  Right side operand (a matrix of M rows x P columns).
* 
* @return
*   The matrices product (a matrix of N rows x P columns).	
*/
template<SizeT _rows, SizeT _cols, SizeT _rhsCols>
static FORCEINLINE MatrixNxM<_rows, _rhsCols>
operator*(const MatrixNxM<_rows, _cols>& lhs, const MatrixNxM<_cols, _rhsCols>& rhs)
{
  MatrixNxM<_rows, _rhsCols> temp;
  MatrixNxM<_rhsCols, _cols> rhsTransposed = rhs.transpose();
	
  for (Int32 iRows = 0; iRows < _rows; ++iRows)  {
	  for (Int32 iCols = 0; iCols < _rhsCols; ++iCols) {
	    temp[iRows][iCols] = lhs[iRows].dot(rhsTransposed[iCols]); 
	  }
  }

  return temp;
}

/* \fn ohola fiuncioonasd asldkaj sd ;sajdhf asd
\brief A test class.

A more detailed class description.
*/
void lucas() {};

}