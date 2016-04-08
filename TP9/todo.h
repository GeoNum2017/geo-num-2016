#ifndef TP9_GEONUM_TODO_H
#define TP9_GEONUM_TODO_H
#include <cmath>

void LoopStep(
    const MatdX3& V0,
    const MatiX3& F0,
          MatdX3& V1,
          MatiX3& F1 )
{
    // 0. COUNTS (# of vertices, faces, edges in the last iteration)
    unsigned int
        v0 = V0.rows(),
        f0 = F0.rows(),
        e0 = v0 + f0 - 2;
        
    //
    // 1. TOPOLOGY SPLIT
    //
    
    // store the adjacency information needed for computation of new positions
    MatiXX VertexAdjacency, MidpointAdjacency;    
    // perform 1-to-4 split
    triangleSplit( F0,V0, F1,V1, VertexAdjacency, MidpointAdjacency );
    //
    // F1 : new topology (4 new triangles per old triangle)
    // V1 : old geometry (first v0 rows) + edge midpoints (next e0 rows)
    //
    // VertexAdjacency :
    //     v0 rows;
    //     column 0       => number of adjacent vertices (= n)
    //     columns 1 to n => indices of adjacent vertices
    //
    // MidpointAdjacency :
    //     e0 rows;
    //     columns 0 and 1 => indices of edge vertices
    //     columns 2 and 3 => indices of opposite vertices
    //
    
    
    //
    // 2. COMPUTE NEW GEOMETRY
    //
    // TODO : update positions for old vertices [vertex mask]
    
    
    // TODO : compute positions for inserted midpoint vertices [edge mask]
    
    
}

#endif
