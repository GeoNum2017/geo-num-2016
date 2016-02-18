#ifndef TP3_GEONUM_TODO_H
#define TP3_GEONUM_TODO_H


Vec2 DeBoor(
    const double t,
    const unsigned int j,
    const unsigned int r,
    const MatX2& ControlPoints,
    const VecX& Knots )
{
    // TODO : calculate and return a B-spline point using De Boor's algorithm
    // hint : make your life simple — recursively!
    //        ...unless you don't like recursion; in which case, just ignore this hint.
    
}


void evaluateBSpline(
    MatX2& SplinePoints,
    const MatX2& ControlPoints,
    const VecX& Knots,
    unsigned int num_samples )
{
    // n+1 control points d_0 ... d_n
    unsigned int n = ControlPoints.rows()-1;
    // m+1 parameter knots t_0 ... t_m
    unsigned int m = Knots.size()-1;
    // degree of the curve d = #knots - #points - 1 = (m+1) - (n+1) - 1
    // which is the same as:
    unsigned int k = m-n-1;
    
    // b-spline curve is defined on [ t_k, t_m-k ) = [ t_k, t_n+1 )
    // this means there are s = n-d+1 segments :
    // [ t_d, t_d+1 ), [ t_d+1, t_d+2 ) ... [ t_n, t_n+1 )
    unsigned int s = n-k+1;
    
    // num_samples points for each segment of the curve
    SplinePoints.resize(s*num_samples,2);
    SplinePoints.fill(0);
    
    // TODO : fill SplinePoints
    // hint : use double loop
    //   - outer loop : iterate over parameter segments [ t_j, t_j+1 ) for k <= j <= n
    //   - inner loop : samples the current segment
    //
    // SplinePoints has the same structure as in the previous TP :
    //  first (num_samples) rows correspond to the 1st segment,
    //  second (num_samples) rows correspond to the 2nd segment,
    //  ...
    //  last (num_samples) rows correspond to the s-th segment.
    //
    // You can use the function t = uniformSampling(a,b,num_samples)
    // to obtain the uniform sampling of interval [a,b]
    // then access the samples simply as: t(i)
    //

}

#endif