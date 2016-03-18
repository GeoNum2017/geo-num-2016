#ifndef EIGEN_TYPEDEFS
#define EIGEN_TYPEDEFS

typedef Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> MatdXX;    // matrix, type double
typedef Eigen::Matrix<double,Eigen::Dynamic,3,Eigen::RowMajor> MatdX3; // matrix with three columns, type double, row major (for OpenGL)
typedef Eigen::Matrix<short ,Eigen::Dynamic,3,Eigen::RowMajor> MatiX3; // matrix with three columns, type short, row major
typedef Eigen::Matrix<double,1,Eigen::Dynamic> VecX;                   // array, type double
typedef Eigen::Matrix<short,1,3> Veci3;                                // 1x3 array, type short

#endif