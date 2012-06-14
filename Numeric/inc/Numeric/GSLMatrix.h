#ifndef NUMERIC_GSLMATRIX_H
#define NUMERIC_GSLMATRIX_H

#include "Numeric/DllExport.h"
#include "Numeric/GSLVector.h"

#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_eigen.h>

#include <vector>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <cstdarg>


namespace Numeric
{

  class GSLMatrix;

  // matrix transpose helper
  struct Tr
  {
    const GSLMatrix& matrix;
    Tr(const GSLMatrix& m):matrix(m){}
  };

  // mutrix multiplication helper
  struct GSLMatrixMult2
  {
    const GSLMatrix& m_1;
    const GSLMatrix& m_2;
    const bool tr1;
    const bool tr2;
    GSLMatrixMult2(const GSLMatrix& m1,const GSLMatrix& m2):
    m_1(m1),m_2(m2),tr1(false),tr2(false){}

    GSLMatrixMult2(const Tr& m1,const GSLMatrix& m2):
    m_1(m1.matrix),m_2(m2),tr1(true),tr2(false){}

    GSLMatrixMult2(const GSLMatrix& m1,const Tr& m2):
    m_1(m1),m_2(m2.matrix),tr1(false),tr2(true){}

    GSLMatrixMult2(const Tr& m1,const Tr& m2):
    m_1(m1.matrix),m_2(m2.matrix),tr1(true),tr2(true){}
  };

  // mutrix multiplication helper
  struct GSLMatrixMult3
  {
    const GSLMatrix& m_1;
    const GSLMatrix& m_2;
    const GSLMatrix& m_3;
    const bool tr1;
    const bool tr2;
    const bool tr3;
    GSLMatrixMult3(const GSLMatrix& m1,const GSLMatrixMult2& mm):
    m_1(m1),m_2(mm.m_1),m_3(mm.m_2),tr1(false),tr2(mm.tr1),tr3(mm.tr2){}

    GSLMatrixMult3(const Tr& m1,const GSLMatrixMult2& mm):
    m_1(m1.matrix),m_2(mm.m_1),m_3(mm.m_2),tr1(true),tr2(mm.tr1),tr3(mm.tr2){}

    GSLMatrixMult3(const GSLMatrixMult2& mm, const GSLMatrix& m2):
    m_1(mm.m_1),m_2(mm.m_2),m_3(m2),tr1(mm.tr1),tr2(mm.tr2),tr3(false){}

    GSLMatrixMult3(const GSLMatrixMult2& mm, const Tr& m2):
    m_1(mm.m_1),m_2(mm.m_2),m_3(m2.matrix),tr1(mm.tr1),tr2(mm.tr2),tr3(true){}
  };

  /**
  An implementation of Jacobian using gsl_matrix.

  */
  class GSLMatrix
  {
    /// The pointer to the GSL's internal jacobian matrix
    gsl_matrix * m_matrix;

  public:
    /// Constructor
    GSLMatrix():m_matrix(NULL){}
    /// Constructor
    GSLMatrix(const size_t nx, const size_t ny)
    {
      m_matrix = gsl_matrix_alloc(nx,ny);
    }

    /// Copy constructor
    GSLMatrix(const GSLMatrix& M)
    {
      m_matrix = gsl_matrix_alloc(M.size1(),M.size2());
      gsl_matrix_memcpy(m_matrix,M.gsl());
    }

    /// Destructor.
    ~GSLMatrix()
    {
      if (m_matrix)
      {
        gsl_matrix_free(m_matrix);
      }
    }

    GSLMatrix& operator=(const GSLMatrix& M)
    {
      resize( M.size1(), M.size2() );
      gsl_matrix_memcpy( m_matrix, M.gsl() );
      return *this;
    }

    /// Get the pointer to the GSL matrix
    gsl_matrix * gsl(){return m_matrix;}

    /// Get the const pointer to the GSL matrix
    const gsl_matrix * gsl() const {return m_matrix;}

    bool isEmpty() const {return m_matrix == NULL;}

    void resize(const size_t nx, const size_t ny)
    {
      if (m_matrix)
      {
        gsl_matrix_free(m_matrix);
      }
      m_matrix = gsl_matrix_alloc(nx,ny);
    }

    /// First size of the matrix
    size_t size1() const {return m_matrix? m_matrix->size1 : 0;}

    /// Second size of the matrix
    size_t size2() const {return m_matrix? m_matrix->size2 : 0;}

    /// set an element
    void set(size_t i, size_t j, double value)
    {
      if (i < m_matrix->size1 && j < m_matrix->size2) gsl_matrix_set(m_matrix,i,j,value);
      else
      {
        throw std::out_of_range("GSLMatrix indices are out of range.");
      }
    }
    /// get an element
    double get(size_t i, size_t j) const
    {
      if (i < m_matrix->size1 && j < m_matrix->size2) return gsl_matrix_get(m_matrix,i,j);
      throw std::out_of_range("GSLMatrix indices are out of range.");
    }

    // Set this matrix to identity matrix
    void identity()
    {
      gsl_matrix_set_identity( m_matrix );
    }

    // Set all elements to zero
    void zero()
    {
      gsl_matrix_set_zero( m_matrix );
    }

    /// Swap the contents of two matrices.
    void swap(GSLMatrix& M)
    {
      std::swap(m_matrix, M.m_matrix);
    }

    // add a matrix to this
    GSLMatrix& operator+=(const GSLMatrix& M)
    {
      gsl_matrix_add( m_matrix, M.gsl() );
      return *this;
    }

    // add a constant to this matrix
    GSLMatrix& operator+=(const double& d)
    {
      gsl_matrix_add_constant( m_matrix, d );
      return *this;
    }

    // subtract a matrix from this
    GSLMatrix& operator-=(const GSLMatrix& M)
    {
      gsl_matrix_sub( m_matrix, M.gsl() );
      return *this;
    }

    // multiply this matrix by a number
    GSLMatrix& operator*=(const double& d)
    {
      gsl_matrix_scale( m_matrix, d );
      return *this;
    }

    GSLMatrix& operator=(const GSLMatrixMult2& mult2)
    {
      // sizes of the result matrix
      size_t n1 = mult2.tr1 ? mult2.m_1.size2() : mult2.m_1.size1();
      size_t n2 = mult2.tr2 ? mult2.m_2.size1() : mult2.m_2.size2();

      this->resize(n1,n2);

      CBLAS_TRANSPOSE tr1 = mult2.tr1? CblasTrans : CblasNoTrans;
      CBLAS_TRANSPOSE tr2 = mult2.tr2? CblasTrans : CblasNoTrans;

      // this = m_1 * m_2
      gsl_blas_dgemm (tr1, tr2,
        1.0, mult2.m_1.gsl(), mult2.m_2.gsl(),
        0.0, gsl());

      return *this;
    }

    GSLMatrix& operator=(const GSLMatrixMult3& mult3)
    {
      // sizes of the result matrix
      size_t n1 = mult3.tr1 ? mult3.m_1.size2() : mult3.m_1.size1();
      size_t n2 = mult3.tr3 ? mult3.m_3.size1() : mult3.m_3.size2();

      this->resize(n1,n2);

      // intermediate matrix 
      GSLMatrix AB( n1, mult3.m_2.size2() );

      CBLAS_TRANSPOSE tr1 = mult3.tr1? CblasTrans : CblasNoTrans;
      CBLAS_TRANSPOSE tr2 = mult3.tr2? CblasTrans : CblasNoTrans;
      CBLAS_TRANSPOSE tr3 = mult3.tr3? CblasTrans : CblasNoTrans;

      // AB = m_1 * m_2
      gsl_blas_dgemm (tr1, tr2,
        1.0, mult3.m_1.gsl(), mult3.m_2.gsl(),
        0.0, AB.gsl());

      // this = AB * m_3
      gsl_blas_dgemm (CblasNoTrans, tr3,
        1.0, AB.gsl(), mult3.m_3.gsl(),
        0.0, gsl());

      return *this;
    }

    /// Solve system of linear equations M*x == rhs, M is this matrix
    /// This matrix is destroyed.
    void solve(const GSLVector& rhs, GSLVector& x)
    {
      if (size1() != size2())
      {
        throw std::runtime_error("System of linear equations: the matrix must be square.");
      }
      size_t n = size1();
      if (rhs.size() != n)
      {
        throw std::runtime_error("System of linear equations: right-hand side vector has wrong size.");
      }
      x.resize(n);
      int s;
      gsl_permutation * p = gsl_permutation_alloc( n );
      gsl_linalg_LU_decomp( gsl(), p, &s ); // matrix is modified at this moment
      gsl_linalg_LU_solve( gsl(), p, rhs.gsl(), x.gsl() );
      gsl_permutation_free( p );
    }

    /// Invert this matrix
    void invert()
    {
      if (size1() != size2())
      {
        throw std::runtime_error("Matrix inverse: the matrix must be square.");
      }
      size_t n = size1();
      int s;
      GSLMatrix LU(*this);
      gsl_permutation * p = gsl_permutation_alloc( n );
      gsl_linalg_LU_decomp( LU.gsl(), p, &s );
      gsl_linalg_LU_invert( LU.gsl(), p, gsl() );
      gsl_permutation_free( p );
    }

    /// Calculate the determinant.
    double det()
    {
      if (size1() != size2())
      {
        throw std::runtime_error("Matrix inverse: the matrix must be square.");
      }
      size_t n = size1();
      int s;
      GSLMatrix LU(*this);
      gsl_permutation * p = gsl_permutation_alloc( n );
      gsl_linalg_LU_decomp( LU.gsl(), p, &s );
      double d = gsl_linalg_LU_det( LU.gsl(), s ); 
      gsl_permutation_free( p );
      return d;
    }

    /**
     * Multiply this matrix by vector x and store result in y. y = M * x
     * @param x :: Vector to multiply with
     * @param y :: The result vector.
     */
    void multiply(const GSLVector& x, GSLVector& y)
    {
      if ( x.size() != size2() )
      {
        throw std::runtime_error("Matrix-vector multiplication: dimension mismatch");
      }
      y.resize( size1() );
      gsl_blas_dgemv( CblasNoTrans, 1.0, gsl(), x.gsl(), 0.0, y.gsl() );
    }

    /**
     * Diagonalize the matrix.
     * @param ev :: Returned eigenvalues.
     * @param ef :: Returned eigenfunctions.
     */
    void diag(GSLVector& ev, GSLMatrix& ef)
    {
      if ( size1() != size2() )
      {
        throw std::runtime_error("Matrix must be square for diag");
      }
      ev.resize( size1() );
      ef.resize( size1(), size1() );
      auto ws = gsl_eigen_symmv_alloc( size1() );
      gsl_eigen_symmv( gsl(), ev.gsl(), ef.gsl(), ws  );
      gsl_eigen_symmv_free( ws );
    }

    /**
     * Diagonalize the matrix as non-symmetric.
     * @param ev :: Returned complex eigenvalues. Vectors size == 2 * size1(). Real parts are
     *  in even positions, imaginary are in odd.
     */
    void diagNonSymm(std::vector<double>& ev)
    {
      if ( size1() != size2() )
      {
        throw std::runtime_error("Matrix must be square for diag");
      }
      size_t n = size1();
      gsl_vector_complex* eval = gsl_vector_complex_alloc( n );
      auto workspace = gsl_eigen_nonsymm_alloc( n );
      gsl_eigen_nonsymm( gsl(), eval, workspace );
      gsl_eigen_nonsymm_free( workspace );
      ev.resize( 2 * n );
      for(size_t i = 0; i < n; ++i )
      {
        auto val = gsl_vector_complex_get( eval, i );
        ev[2*i] = GSL_REAL( val );
        ev[2*i + 1] = GSL_IMAG( val );
      }
    }

    /**
     * Set a row of a small matrix. Number of variadic arguments must be == size2()
     * @param row :: Row index.
     */
    void setRow(size_t row, ...)
    {
      if ( !m_matrix ) return;
      if (row >= m_matrix->size1)
      {
        throw std::out_of_range("GSLMatrix row index is out of range.");
      }
      va_list args;
      va_start(args, row);
      for(size_t col = 0; col < size2(); ++col)
      {
        double value = va_arg(args, double);
        gsl_matrix_set( gsl(), row, col, value );
      }
    }

  };// class GSLMatrix

  inline GSLMatrixMult2 operator*(const GSLMatrix& m1, const GSLMatrix& m2)
  {
    return GSLMatrixMult2(m1,m2);
  }

  inline GSLMatrixMult2 operator*(const Tr& m1, const GSLMatrix& m2)
  {
    return GSLMatrixMult2(m1,m2);
  }

  inline GSLMatrixMult2 operator*(const GSLMatrix& m1, const Tr& m2)
  {
    return GSLMatrixMult2(m1,m2);
  }

  inline GSLMatrixMult2 operator*(const Tr& m1, const Tr& m2)
  {
    return GSLMatrixMult2(m1,m2);
  }

  inline GSLMatrixMult3 operator*(const GSLMatrix& m, const GSLMatrixMult2& mm)
  {
    return GSLMatrixMult3(m,mm);
  }

  inline GSLMatrixMult3 operator*( const GSLMatrixMult2& mm, const GSLMatrix& m )
  {
    return GSLMatrixMult3(mm,m);
  }

  inline GSLMatrixMult3 operator*(const Tr& m, const GSLMatrixMult2& mm)
  {
    return GSLMatrixMult3(m,mm);
  }

  inline GSLMatrixMult3 operator*( const GSLMatrixMult2& mm, const Tr& m )
  {
    return GSLMatrixMult3(mm,m);
  }

  inline std::ostream& operator<<(std::ostream& ostr, const GSLMatrix& m)
  {
    ostr << std::scientific << std::setprecision(6);
    for(size_t i = 0; i < m.size1(); ++i)
    {
      for(size_t j = 0; j < m.size2(); ++j)
      {
        ostr << std::setw(13) << m.get(i,j) << ' ';
      }
      ostr << std::endl;
    }
    return ostr;
  }

} // NUMERIC

#endif // NUMERIC_GSLMATRIX_H
