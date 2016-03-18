#ifndef TP6_GEONUM_TODO_H
#define TP6_GEONUM_TODO_H
/* 
 * 
 *uniform sampling of the interval [0,1] 
 * 
 */
VecX uniformSampling(unsigned int num_samples)
{
    VecX samples( num_samples );
    float l = 1.0 / (float)(num_samples-1);
    for(unsigned int i = 0; i < num_samples; ++i)
        samples(i) = i*l;
    return samples;
}
/*
 * 
 * Casteljau : tensor product version
 * 
*/
double CasteljauPoint(
    const MatdXX coord,
    const unsigned int m,
    const unsigned int n,
    const unsigned int i,
    const unsigned int j,
    const double u,
    const double v )
{   
    // 
    // TODO : implement the De Casteljau algorithm for Bezier surfaces
    //
}
/*
 * 
 * Compute Bezier Patch
 * 
*/
bool BezierPatch(
    MatdXX &X,
    MatdXX &Y,
    MatdXX &Z,
    const MatdXX &netX,
    const MatdXX &netY,
    const MatdXX &netZ,
    unsigned int num_samples )
{
    //
    // netX, netY, netZ : control net of the Bezier patch, each matrix holds one coordinate, their size is (degU+1) x (degV+1)
    //    X,    Y,    Z : points on the surface, to compute, each matrix holds one coordinate, their size will be (num_samples) x (num_samples)
    //
    
    // degrees of the patch
    unsigned int degU = netX.rows()-1;
    unsigned int degV = netX.cols()-1;
    
    // check if matrices have the correct size
    assert( (degU+1)==netY.rows() && (degU+1)==netZ.rows() &&  (degV+1)==netY.cols() && (degV+1)==netZ.cols() );
    
    // prepare the matrices, we're gonna have num_samples^2 points
    X.setZero(num_samples,num_samples);
    Y.setZero(num_samples,num_samples);
    Z.setZero(num_samples,num_samples);
    
    // uniform sampling of the interval [0,1]
    // access with t(i)
    VecX t = uniformSampling(num_samples);

    //
    // TODO : save surface points into coordinate matrices X, Y, Z
    // ...use double loop for rows & cols of the matrices
    //
}
#endif