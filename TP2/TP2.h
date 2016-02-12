#ifndef TP2_GEONUM_H
#define TP2_GEONUM_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <cmath>
#include <cstdio>
#include <cstdlib>

#include <Eigen/Dense> // Eigen quick reference: http://eigen.tuxfamily.org/dox-devel/AsciiQuickReference.txt


typedef Eigen::Matrix<double,Eigen::Dynamic,2> MatX2; // matrix with two columns
typedef Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> MatXX;
typedef Eigen::Matrix<double,1,2> Vec2;               // point/vector with two coordinates
typedef Eigen::Matrix<double,1,Eigen::Dynamic> VecX;  // array


/* uniform sampling of interval [0,1] */
VecX uniformSampling(unsigned int num_samples)
{
    VecX samples( num_samples );
    float l = 1.0 / (float)(num_samples-1);
    for(unsigned int i = 0; i < num_samples; ++i)
        samples(i) = i*l;
    return samples;
}

/* De Casteljau algorithm - for a fixed t */
Vec2 CasteljauPoint(
    const MatX2& BP,
    const unsigned int n,
    const unsigned int i,
    const float t )
{
    if(n==0)
        return BP.row(i);
    else
        return (1-t) * CasteljauPoint(BP,n-1,i,t) + t * CasteljauPoint(BP,n-1,i+1,t);
}
/* evaluate curve points via De Casteljau */
bool CasteljauCurve( 
    MatX2& C,
    const MatX2& BP,
    const unsigned int num_samples )
{
    C.resize(num_samples,2);
    C.fill(0);
    VecX samples = uniformSampling(num_samples);
    for(int i=0; i < C.rows(); ++i)
        C.row(i) = CasteljauPoint(BP,BP.rows()-1,0,samples(i));
    
    return true;
}

/* read Bezier points (BP) from a file */
bool readBCV(
    std::string filename,
    MatX2& BP )
{   
    std::ifstream inputFile;
    std::string line;
    std::stringstream ss;
    
    inputFile.open(filename);
    if(!inputFile.is_open()) return false;
    
    // get first line = degree of the curve
    std::getline(inputFile,line);
    ss << line;
    unsigned int degree;
    ss >> degree;
    
    // resize the matrix
    BP.resize(degree+1,2);
    BP.fill(0);
    
    for(unsigned int i=0;i < degree+1; i++)
    {
        // get coordinates of next Bezier point
        std::getline(inputFile,line);
        ss << line;
        double x, y;
        ss >> x >> y;
        // store in Eigen matrix
        BP(i,0) = x;
        BP(i,1) = y;
    }
    inputFile.close();
    
    std::cout << BP.rows() << " control points read from " << filename << std::endl << BP << std::endl;
    
    return true;
}

/* write data stored as an Eigen matrix */
bool writeMatrixData(
    std::string filename,
    const MatX2& C )
{
    std::ofstream outputFile;
    
    outputFile.open(filename);
    if(!outputFile.is_open()) return false;
    
    outputFile.precision(8);
    outputFile << C;
    outputFile.close();
    
    std::cout << C.rows() << " points written to " << filename << std::endl;
    
    return true;
}

/* write spline data */
bool writeSplineData(
    const MatX2& SplinePoints,
    const unsigned int curve_count,
    const unsigned int num_samples,
    const std::string out_dir
)
{
    for(unsigned int i = 0; i < curve_count; i++)
    {
        std::string out_filename = out_dir + "curve" + std::to_string(i) + ".txt";
        
        MatX2 C = SplinePoints.middleRows(num_samples*i,num_samples);
        
        if(!writeMatrixData(out_filename,C)) {
            std::cerr << "ERROR: failed to write " << out_filename << std::endl;
            return false;
        }
    }
    return true;
}

#endif
