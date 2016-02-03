#ifndef TP1_GEONUM_H
#define TP1_GEONUM_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <ctime>
#include <cstdio>
#include <cstdlib>

#include <vector>

#include <Eigen/Dense>

/* uniform sampling of interval [0,1] */
std::vector<float> uniformSampling(unsigned int num_samples)
{
    std::vector<float> samples( num_samples );
    float l = 1.0 / (float)(num_samples-1);
    for(unsigned int i = 0; i < num_samples; ++i)
        samples[i] = i*l;
    return samples;
}

/* factorial computation */
unsigned int fact( unsigned int n )
{
    unsigned int f = 1;
    for(unsigned int i = 0; i < n; ++i)
        f = f*(n-i);
    return f;
}

/* combinatorial number */
unsigned int combNumber(unsigned int n, unsigned int i)
{
    return fact(n) / (fact(n-i) * fact(i));
}

/* read Bezier points (BP) from a file */
bool readBCV(
    const char * filename,
    Mat2f& BP )
{   
    std::ifstream inputFile;
    std::string line;
    std::stringstream ss;
    
    inputFile.open(filename);
    // get first line = degree of the curve
    std::getline(inputFile,line);
    ss << line;
    unsigned int degree;
    ss >> degree;
    
    // resize the matrix
    BP.resize(degree+1,2);
    
    for(unsigned int i=0;i < degree+1; i++)
    {
        // get coordinates of next Bezier point
        std::getline(inputFile,line);
        ss << line;
        float x, y;
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
    const char * filename,
    const Mat2f& C )
{
    std::ofstream outputFile;
    
    outputFile.open(filename);
    outputFile.precision(8);
    outputFile << C;
    outputFile.close();
    
    std::cout << C.rows() << " points written to " << filename << std::endl;
    
    return true;
}

/* measure time */
double measureTime( 
    std::clock_t start, 
    bool write_output = true )
{
    double duration = (std::clock() - start) / (double) CLOCKS_PER_SEC * 1000; // in ms
    
    if(write_output)
        std::cout << duration << " ms" << std::endl;
    
    return duration;
}

#endif
