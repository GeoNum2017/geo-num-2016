#ifndef TP3_GEONUM_H
#define TP3_GEONUM_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <cmath>
#include <cstdio>
#include <cstdlib>

#include <Eigen/Dense> 
// Eigen quick reference: http://eigen.tuxfamily.org/dox-devel/AsciiQuickReference.txt

typedef Eigen::Matrix<double,Eigen::Dynamic,2> MatX2; // matrix with two columns
typedef Eigen::Matrix<double,Eigen::Dynamic,3> MatX3; // matrix with three columns
typedef Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> MatXX;
typedef Eigen::Matrix<double,1,2> Vec2;               // point/vector with two coordinates
typedef Eigen::Matrix<double,1,3> Vec3;               // point/vector with three coordinates
typedef Eigen::Matrix<double,1,Eigen::Dynamic> VecX;  // array


/* uniformly sample interval [a,b] */
VecX uniformSampling(double a,double b,unsigned int num_samples)
{
    VecX samples( num_samples );
    double l = (b-a) / (double)(num_samples-1);
    for(unsigned int i = 0; i < num_samples; ++i)
        samples(i) = a + i*l;
    return samples;
}

/* read B-spline control points and nodes */
template <typename MatXd>
bool readBSpline(
    std::string filename,
    MatXd& ControlPoints,
    VecX& Nodes )
{   
    std::ifstream inputFile;
    std::string line;
    std::stringstream ss;
    
    inputFile.open(filename);
    if(!inputFile.is_open()) return false;
    
    std::getline(inputFile,line);
    ss << line;
    unsigned int num_points;
    ss >> num_points;
    
    // resize the ControlPoints
    unsigned int d = ControlPoints.cols();
    ControlPoints.resize(num_points,d);
    ControlPoints.fill(0);
    
    for(unsigned int i=0;i < num_points; i++)
    {
        // get coordinates of next control point
        std::getline(inputFile,line);
        ss << line;
        for(unsigned int j=0; j<d; j++)
            ss >> ControlPoints(i,j);
    }
    
    std::getline(inputFile,line);
    ss << line;
    unsigned int num_nodes;
    ss >> num_nodes;
    
    // resize the Nodes
    Nodes.resize(num_nodes);
    Nodes.fill(0);

    for(unsigned int i=0;i < num_nodes; i++)
    {
        // get next node
        std::getline(inputFile,line);
        ss << line;
        ss >> Nodes(i);
    }

    inputFile.close();
    
    std::cout << "B-spline " << filename << " read.\n"
              << "degree=" << Nodes.size()-ControlPoints.rows()-1 << "\n" // d = m-n-1
              << ControlPoints.rows() << " control points :\n"
              << ControlPoints << std::endl
              << Nodes.size() << " nodes :\n"
              << Nodes << std::endl;
    
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
    const unsigned int num_samples,
    const std::string out_dir )
{
    const unsigned int curve_count = SplinePoints.rows() / num_samples;
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
