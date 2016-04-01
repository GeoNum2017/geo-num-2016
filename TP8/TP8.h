#ifndef TP8_GEONUM_H
#define TP8_GEONUM_H
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

bool readControlPoints(
    std::string filename,
    MatdXX& X,
    MatdXX& Y,
    MatdXX& Z,
    bool& closedU,
    bool& closedV )
{   
    std::ifstream inputFile(filename.c_str());
    if(!inputFile.is_open()) 
        return false;
    
    std::string line;
    // get first line
    std::getline(inputFile,line);
    
    std::stringstream ss0(line);
    unsigned int numPu, numPv;
    ss0 >> numPu >> numPv >> closedU >> closedV;
    
    X.setZero( numPu, numPv );
    Y.setZero( numPu, numPv );
    Z.setZero( numPu, numPv );
    
    for(unsigned int j=0;j < numPv; j++) {
        for(unsigned int i=0;i < numPu; i++) {
            // get coordinates of next Bezier point
            std::getline(inputFile,line);
            std::stringstream ss(line);
            double x, y, z;
            ss >> x >> y >> z;
            // store in Eigen matrices
            X(i,j) = x;
            Y(i,j) = y;
            Z(i,j) = z;
    }}
    inputFile.close();
    
    // stats
    std::cout << filename << " read.\n"
              << X.size() << " control points :\n\n"
              << "X = [\n" << X << "\n]\n\n"
              << "Y = [\n" << Y << "\n]\n\n" 
              << "Z = [\n" << Z << "\n]\n\n"
              << "closed_u=" << closedU << "\n\n"
              << "closed_v=" << closedV << "\n\n"
              ;
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
    
    unsigned int numKv = countU*countV;
    unsigned int numF = 2*(countU-1)*(countV-1);
    
    V.setZero(numKv,3);
    F.setZero(numF,3);
    
    MatdXX co;
    co = X; co.resize(numKv,1); V.col(0) = co;                        
    co = Y; co.resize(numKv,1); V.col(1) = co;
    co = Z; co.resize(numKv,1); V.col(2) = co;

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
