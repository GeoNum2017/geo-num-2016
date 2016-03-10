#ifndef TP5_GEONUM_H
#define TP5_GEONUM_H

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

template <typename MatXd>
bool readData(
    const std::string filename,
    MatXd& Points )
{   
    std::ifstream inputFile;
    std::string line;
    std::stringstream ss;
    
    // open the file
    inputFile.open(filename);
    if(!inputFile.is_open()) return false;
    
    // get number of points
    std::getline(inputFile,line);
    ss << line;
    unsigned int num_points;
    ss >> num_points;
    
    // resize the Points
    // but first, get dimension
    unsigned int d = Points.cols();
    Points.resize(num_points,d);
    Points.fill(0);
    
    // read points
    for(unsigned int i=0;i < num_points; i++)
    {
        // get coordinates of next point
        std::getline(inputFile,line);
        ss << line;
        for(unsigned int j=0; j<d; j++)
            ss >> Points(i,j);
    }
    
    // close the file
    inputFile.close();
    
    // stats
    std::cout << filename << " read.\n"
              << Points.rows() << " control points :\n"
              << Points.format(Eigen::IOFormat(4,0,",","\n","  [","]")) << std::endl;
    
    return true;
}

bool writeData(
    const std::string filename,
    const MatX2& Points )
{
    std::ofstream outputFile;
    outputFile.open(filename);
    if(!outputFile.is_open()) return false;
    outputFile.precision(8);
    outputFile << Points;
    outputFile.close();
    std::cout << Points.rows() << " points written to " << filename << std::endl;
    return true;
}

#endif
