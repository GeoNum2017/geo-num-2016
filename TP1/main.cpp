#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <cstdio>
#include <cstdlib>

#include <Eigen/Dense> // http://eigen.tuxfamily.org/dox-devel/AsciiQuickReference.txt
typedef Eigen::Matrix<float,Eigen::Dynamic,2> Mat2f; // matrix with two columns
typedef Eigen::Vector2f Vec2f;                       // point/vector with two coordinates

typedef std::vector<float> vecf;

#include "TP1.h"
    
void BernsteinCurve(
    Mat2f& C,
    const Mat2f& BP, 
    const unsigned int num_samples )
{
    C.resize(num_samples,2);
    C.fill(0);
    vecf samples = uniformSampling(num_samples);
    unsigned int degree = BP.rows()-1;
    
    /* TODO : implement curve point computation via Bernstein polynomials */
}

void CasteljauCurve( 
    Mat2f& C,
    const Mat2f& BP,
    const unsigned int num_samples )
{
    C.resize(num_samples,2);
    C.fill(0);
    vecf samples = uniformSampling(num_samples);
    unsigned int degree = BP.rows()-1;
    
    /* TODO : implement the De Casteljau's algorithm */

}

int main()
{
    // define number of samples to be computed
    unsigned int num_samples = 100;
    
    // input files
    const char * i_simple    = "../data/simple.bcv";
    const char * i_spiral    = "../data/spiral.bcv";
    const char * i_infinity  = "../data/infinity.bcv";
    
    // output files
    const char * o_bernstein = "../plots/bernstein.txt";
    const char * o_casteljau = "../plots/casteljau.txt";
    
    // Bezier points = control polygon, (n+1)x2 matrix
    Mat2f BP;
    // curve points
    Mat2f C_Casteljau, C_Bernstein;
    
    // read Bezier points
    if( !readBCV(i_infinity,BP) ) return -1;
        
    // compute curve points using Bernstein basis
    std::clock_t start = std::clock();
    BernsteinCurve( C_Bernstein, BP, num_samples );
    measureTime(start);
    
    // compute curve points using DeCasteljau
    start = std::clock();
    CasteljauCurve( C_Casteljau, BP, num_samples );
    measureTime(start);
    
    // export curves
    //if( !writeMatrixData(o_bernstein,C_Bernstein) ) return -1;
    //if( !writeMatrixData(o_casteljau,C_Casteljau) ) return -1;

    return 0;
}
