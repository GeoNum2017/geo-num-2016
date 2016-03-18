#ifndef TP6_GEONUM_H
#define TP6_GEONUM_H
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <Eigen/Dense>
#include <typedefs.h>
#include <vector>

/* read Bezier points (BP) from a file */
bool readBPT(
    std::string filename,
    std::vector<MatdXX>& X,
    std::vector<MatdXX>& Y,
    std::vector<MatdXX>& Z )
{   
    std::ifstream inputFile(filename.c_str());
    if(!inputFile.is_open()) 
        return false;
    
    std::string line;
    // get first line = number of patches
    std::getline(inputFile,line);
    std::stringstream ss0(line);
    
    unsigned int patch_count;
    ss0 >> patch_count;
    
    X.clear();
    Y.clear();
    Z.clear();
        
    for(unsigned int p=0; p < patch_count; p++)
    {
        // get first line fir current patch = degrees of the patch
        std::getline(inputFile,line);
        std::stringstream ss1(line);
        unsigned int degU, degV;
        ss1 >> degU >> degV;
        
        MatdXX nX, nY, nZ;
        nX.setZero(degU+1,degV+1);
        nY.setZero(degU+1,degV+1);
        nZ.setZero(degU+1,degV+1);
        
        for(unsigned int i=0;i < degU+1; i++) {
            for(unsigned int j=0;j < degV+1; j++) {
                // get coordinates of next Bezier point
                std::getline(inputFile,line);
                std::stringstream ss2(line);
                double x, y, z;
                ss2 >> x >> y >> z;
                // store in Eigen matrices
                nX(i,j) = x;
                nY(i,j) = y;
                nZ(i,j) = z;
        }}
        X.push_back(nX);
        Y.push_back(nY);
        Z.push_back(nZ);
    }
    inputFile.close();
    
    // stats
    return true;
}

bool patchXYZtoVF(
    const MatdXX& X,
    const MatdXX& Y,
    const MatdXX& Z,
          MatdX3& V,
          MatiX3& F )
{
    unsigned int countU = X.rows();
    unsigned int countV = X.cols();
    
    // check if matrices have the correct size
    assert( countU==Y.rows() && countU==Z.rows() &&  countV==Y.cols() && countV==Z.cols() );
    
    unsigned int numV = countU*countV;
    unsigned int numF = 2*(countU-1)*(countV-1);
    
    V.setZero(numV,3);
    F.setZero(numF,3);
    
    MatdXX co;
    co = X; co.resize(numV,1); V.col(0) = co;                        
    co = Y; co.resize(numV,1); V.col(1) = co;
    co = Z; co.resize(numV,1); V.col(2) = co;

    for(unsigned int i=0; i<countU-1; i++) {
        for(unsigned int j=0; j<countV-1; j++) {
            short A = i*countU+j+0;
            short B = A+1;
            short C = (i+1)*countU+j+0;
            short D = C+1;
            F.row( 2* (i*(countU-1) + j) + 0 ) = Veci3(A,B,D);
            F.row( 2* (i*(countU-1) + j) + 1 ) = Veci3(D,C,A);
        }
    }
}

bool writePatch(
    const std::string filename,
    const MatdXX& X,
    const MatdXX& Y,
    const MatdXX& Z )
{
    MatdX3 V;
    MatiX3 F;
    patchXYZtoVF(X,Y,Z,V,F);
    
    std::ofstream outputFile;
    outputFile.open(filename.c_str());
    if(!outputFile.is_open()) return false;
    outputFile.precision(8);
    
    outputFile <<
        "OFF\n" << V.rows()<< " " << F.rows() << " 0\n" <<
        V.format(Eigen::IOFormat(Eigen::FullPrecision,0," ","\n","","","","\n")) << 
        (F.array()).format(Eigen::IOFormat(Eigen::FullPrecision,0," ","\n","3 ","","","\n"));

    outputFile.close();
    return true;
}


#endif
