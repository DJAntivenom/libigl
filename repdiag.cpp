#include "repdiag.h"

template <typename T>
IGL_INLINE void igl::repdiag(
  const Eigen::SparseMatrix<T>& A,
  const int d,
  Eigen::SparseMatrix<T>& B)
{
  int m = A.rows();
  int n = A.cols();

  // Should be able to *easily* do this in coherent order without
  // dynamicsparsematrix

  Eigen::DynamicSparseMatrix<T, Eigen::RowMajor> 
    dyn_B(m*d,n*d);
  // Reserve enough space for new non zeros
  dyn_B.reserve(d*A.nonZeros());

  // loop over reps
  for(int i=0;i<d;i++)
  {
    // Loop outer level
    for (int k=0; k<A.outerSize(); ++k)
    {
      // loop inner level
      for (typename Eigen::SparseMatrix<T>::InnerIterator it(A,k); it; ++it)
      {
        dyn_B.coeffRef(i*m+it.row(),i*n+it.col()) += it.value();
      }
    }
  }

  B = Eigen::SparseMatrix<T>(dyn_B);
}

template <typename T>
IGL_INLINE void igl::repdiag(
  const Eigen::Matrix<T,Eigen::Dynamic,Eigen::Dynamic> & A,
  const int d,
  Eigen::Matrix<T,Eigen::Dynamic,Eigen::Dynamic> & B)
{
  int m = A.rows();
  int n = A.cols();
  B.resize(m*d,n*d);
  B.array() *= 0;
  for(int i = 0;i<d;i++)
  {
    B.block(i*m,i*n,m,n) = A;
  }
}

// Wrapper with B as output
template <class Mat>
IGL_INLINE Mat igl::repdiag(const Mat & A, const int d)
{
  Mat B;
  repdiag(A,d,B);
  return B;
}

#ifndef IGL_HEADER_ONLY
// Explicit template specialization
#endif