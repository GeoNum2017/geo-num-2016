#ifndef TP7_GEONUM_TODO_H
#define TP7_GEONUM_TODO_H

//
// DeBoor_curve() :
//   recursive implementation of DeBoor for curves
//   use this if your implementation for curves did not work,
//   or just as an ispiration.
//
Vec3 DeBoor_curve(
    double t,
    const unsigned int j,
    const unsigned int r,
    const MatX3& ControlPoints,
    const VecX& Knots )
{
    if( r == 0 )
        return ControlPoints.row(j);
    else {
        unsigned int n = ControlPoints.rows()-1; // n+1 control points d_0 ... d_n
        unsigned int m = Knots.size()-1;         // m+1 nodes t_0 ... t_m
        unsigned int k = m-n-1;                  // degree of the curve
        double w = ( t - Knots(j) ) / ( Knots(j+k-r+1) - Knots(j) );
        return
            w * DeBoor3(t,j  ,r-1,ControlPoints,Knots) 
        +(1-w)* DeBoor3(t,j-1,r-1,ControlPoints,Knots);
    }
}



void evaluateBSpline(
          MatdXX& X,    // output surface patches : X-coord
          MatdXX& Y,    //                          Y-coord
          MatdXX& Z,    //                          Z-coord
    const MatdXX& netX, // input control network :  X-coord
    const MatdXX& netY, //                          Y-coord
    const MatdXX& netZ, //                          Z-coord
    const VecX& knotsU, // input knot vector : u direction
    const VecX& knotsV, // input knot vector : v direction
    unsigned int num_samples // number of samples par patch will be num_samples^2
){
    unsigned int 
        m = netX.rows()-1,
        n = netX.cols()-1,
        k = nodesU.size()-(m+1)-1,
        l = nodesV.size()-(n+1)-1,
        rows = num_samples * (m-k+1), // number of rows in the big output matrices X, Y, Z
        cols = num_samples * (n-l+1); // number of cols in the big output matrices X, Y, Z
    
    X.setZero(rows,cols); // setZero() : resize matrix to given dimensions and set all entries to 0.
    Y.setZero(rows,cols);
    Z.setZero(rows,cols);

    // TODO : implement evaluation of B-spline surfaces.
    //
    // option 1: 
    //    Same way we did for Bezier surfaces.
    //    Evaluate first in u direction >>> control points for a B-spline curve >>> evaluate this curve in the other direction.
    // 
    // option 2:
    //    Do everything recursively.
    //    As an inspiration, you can use the above curve function DeBoor_curve()
    //
}

#endif
