#ifndef TP9_GEONUM_H
#define TP9_GEONUM_H
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <streambuf>
#include <Eigen/Dense>
#include <typedefs.h>
#include <vector>

void triangleSplit(
    const MatiX3& F0,
    const MatdX3& V0,
          MatiX3& F1,
          MatdX3& V1,
          MatiXX& A,
          MatiXX& M )
{
    unsigned int
        v0 = V0.rows(),
        v1 = v0,
        f0 = F0.rows(),
        f1 = 4*f0,
        e0 = v0 + f0 - 2; // from V-E+F=2    
    
    // mesh topology in one matrix
    MatiXX T;
    T.resize(f0,6);
    T.fill(-9);
    
    // fill with face vertex indices
    T.col(0) = F0.col(0);
    T.col(2) = F0.col(1);
    T.col(4) = F0.col(2);
    
    // all possible (oriented) edges
    MatiXX E;
    E.resize(v0+1,v0+1);
    E.fill(-99);
    
    // vertex-vertex adjacency list
    // - 0    : number of vertices in the 1-ring
    // - 1-end: indices of neighbors
    A.setZero(v0,20);
    A.fill(-999);
    A.col(0) *= 0; // set the counter to 0
    
    // midpoint data : each row corresponds to one generated midpoint
    //  0 : adjacent vertex 
    //  1 : adjacent vertex 
    //  2 : opposite vertex 
    //  3 : opposite vertex 
    M.setZero(e0,4);
    
    // init new vertices
    V1.setZero(v0+e0,3);
    // include old positions
    V1.topRows(v0) = V0;
    
    // for each oriented edge (there is 2*e0 of them) :
    unsigned int index;
    for(unsigned int counter=0,index=1;counter<2*e0;counter++,index+=2)
    {
        // extract indices
        unsigned int
            face=index/6,     // face index
            prev=(index-1)%6, // indices of edge vertices in T
            next=(index+1)%6, // indices of opposite vertex in T
            opp =(index+3)%6; // same as (index-3)%6
            
        unsigned int
            p0=T(6*face+prev), // index of 1st edge vertex in V0
            p1=T(6*face+next), // index of 2nd edge vertex in V0
            o =T(6*face+opp);  // index of opposite vertex in V0
        
        if( E(p0,p1) == -99 )
        {   // we have the first oriented edge, need to generate new midpoint
            //
            // change helper edge structure
            E(p0,p1) = v1;
            E(p1,p0) = v1;
            //
            // set position as midpoint
            V1.row(v1) = 0.5*(V0.row(p0)+V0.row(p1));
            //
            // store data needed later
            // - adjacent vertices
            M(E(p0,p1)-v0,0) = p0;
            M(E(p0,p1)-v0,1) = p1;
            // - first opposite vertex
            M(E(p0,p1)-v0,2) = o;
            //
            // push back to vertex adjacency lists
            A(p0,++A(p0,0)) = p1;
            A(p1,++A(p1,0)) = p0;
            //
            // increment vertex count
            v1++;
        }
        else
        {   // second oriented edge, don't generate new midpoint, use the existing one
            //
            // store data needed later
            // - second opposite vertex
            M(E(p0,p1)-v0,3) = o;
        }
        // update topology
        T(index) = E(p0,p1);
    }
    // trim unneeded cols
    A.conservativeResize(A.rows(),A.col(0).array().maxCoeff()+1);
    // generate new faces : extract from topology matrix T
    F1.resize(f1,3);
    F1 << 
        T.col(1), T.col(2), T.col(3),
        T.col(3), T.col(4), T.col(5),
        T.col(5), T.col(0), T.col(1),
        T.col(1), T.col(3), T.col(5);
}

bool readOFF(
    std::string filename,
    MatdX3& V,
    MatiX3& F )
{   
    std::ifstream inputFile(filename.c_str());
    if(!inputFile.is_open()) 
        return false;
    std::string line;
    // 1st line = OFF
    std::getline(inputFile,line);
    // 2nd line = counts (V,F,0)
    std::getline(inputFile,line);
    std::stringstream ss0(line);
    unsigned int vc, fc, tri;
    ss0 >> vc >> fc;
    V.setZero(vc,3);
    F.setZero(fc,3);
    // V : read vertices
    for(unsigned int v=0;v < vc; v++) {
        // get coordinates of next vertex
        std::getline(inputFile,line);
        std::stringstream ss(line);
        ss >> V(v,0) >> V(v,1) >> V(v,2);
    }
    // F : read faces
    for(unsigned int f=0;f < fc; f++) {
        // get indices of next face
        std::getline(inputFile,line);
        std::stringstream ss(line);
        ss >> tri >> F(f,0) >> F(f,1) >> F(f,2);
    }
    inputFile.close();
    std::cout << "Mesh read.\n" << 
                 "  #V0 = " << V.rows() << std::endl <<
                 "  #F0 = " << F.rows() << std::endl; 
    return true;
}

bool writeOFF(
    const std::string filename,
    const MatdX3& V,
    const MatiX3& F )
{    
    std::ofstream outputFile;
    outputFile.open(filename.c_str());
    if(!outputFile.is_open())
        return false;
    outputFile.precision(8);
    outputFile <<
        "OFF\n" << V.rows()<< " " << F.rows() << " 0\n" <<
        V.format(Eigen::IOFormat(Eigen::FullPrecision,0," ","\n","","","","\n")) << 
        (F.array()).format(Eigen::IOFormat(Eigen::FullPrecision,0," ","\n","3 ","","","\n"));
    outputFile.close();
    return true;
}


#endif
