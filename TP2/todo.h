#ifndef TP2_GEONUM_TODO_H
#define TP2_GEONUM_TODO_H

bool computeSplineC1(
    MatX2& ControlPointsC1,
    const MatX2& InputPoints )
{
    unsigned int n = InputPoints.rows()-1; // n+1 input points p_0, p_1, ... p_n .
    
    ControlPointsC1.resize(2*n+1,2); // 2*n+1 control points : n+1 input points, n interior points
    ControlPointsC1.fill(0);
    
    // TODO : Fill the control points
    // hint : set the first interior point to 0.5 * (InputPoints.row(0) + InputPoints.row(1))
    

    
    return true;
}


bool evaluateSpline(
    MatX2& SplinePoints,
    const MatX2& ControlPoints,
    const int degree,
    const int num_samples )
{
    unsigned int n = (ControlPoints.rows()-1) / degree; // n = number of curves
    SplinePoints.resize( num_samples * n, 2);
    
    // TODO : Evaluate the spline points for each curve.
    // hint : retrieve the control points of a processed curve from ControlPoints and pass them to CasteljauCurve.
    //        Then, store the evaluated points in appropriate rows in SplinePoints.
    for(unsigned int i = 0; i < n; i++)
    {
        
    }
    return true;
}


bool computeSplineC2(
    MatX2& ControlPointsC2,
    const MatX2& InputPoints )
{
    unsigned int n = InputPoints.rows()-1; // n+1 input points p_0, p_1, ... p_n .
    
    // Matrix of the linear system
    MatXX A;
    A.resize(3*n+1,3*n+1);
    A.fill(0);
    
    // TODO : Construct the linear system matrix A.
    // hint : only modify the non-zero coefficients
    //     C0 : n+1 rows
    //     C1 : n-1 rows
    //     C2 : n-1 rows
    //     natural spline : 2 rows
    
    
    
    // Right side
    MatX2 B;
    B.resize(3*n+1,2);
    B.fill(0);
    
    // TODO : Construct the right side B.
    // hint : again, only modify the non-zero coefficients (C0 constraints)
    

    
    // Solve the system
    // TODO : Uncomment the following line when A and B are ready.
    //ControlPointsC2 = A.colPivHouseholderQr().solve(B);
    
    return true;
}

#endif