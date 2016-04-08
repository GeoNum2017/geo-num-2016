#ifndef EIGEN_TYPEDEFS
#define EIGEN_TYPEDEFS

typedef Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> MatdXX;    // matrix, type double
typedef Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> MatXX;     // matrix, type double

typedef Eigen::Matrix<double,Eigen::Dynamic,2> MatX2;                  // matrix with two columns, type double, column major
typedef Eigen::Matrix<double,Eigen::Dynamic,3> MatX3;                  // matrix with three columns, type double, column major

typedef Eigen::Matrix<double,Eigen::Dynamic,3,Eigen::RowMajor> MatdX3; // matrix with three columns, type double, row major (for OpenGL)

typedef Eigen::Matrix<short ,Eigen::Dynamic,2,Eigen::RowMajor> MatiX2; // matrix with two columns, type short, row major
typedef Eigen::Matrix<short ,Eigen::Dynamic,3,Eigen::RowMajor> MatiX3; // matrix with three columns, type short, row major
typedef Eigen::Matrix<short ,Eigen::Dynamic,6,Eigen::RowMajor> MatiX6; // matrix with six columns, type short, row major
typedef Eigen::Matrix<short ,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> MatiXX; // matrix with six columns, type short, row major

typedef Eigen::Matrix<double,1,Eigen::Dynamic> VecX;                   // array, type double
typedef Eigen::VectorXi VecXi;                                         // array, type int
typedef Eigen::Matrix<double,1,Eigen::Dynamic> VecdX;                  // array, type double
typedef Eigen::Matrix<short,1,3> Veci3;                                // 1x3 array, type short

typedef Eigen::Matrix<double,1,2> Vec2;                                // point/vector with two coordinates
typedef Eigen::Matrix<double,1,3> Vec3;                                // point/vector with three coordinates

typedef Eigen::Matrix<bool,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> MatB; // bool

#endif
