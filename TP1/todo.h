#ifndef TP1_GEONUM_TODO_H
#define TP1_GEONUM_TODO_H

void BernsteinCurve(
    MatX2& C,
    const MatX2& BP, 
    const unsigned int num_samples )
{
    C.resize(num_samples,2);
    C.fill(0);
    VecX samples = uniformSampling(num_samples);
    unsigned int degree = BP.rows()-1;
    
    /* TODO : implement curve point computation via Bernstein polynomials */
}

void CasteljauCurve( 
    MatX2& C,
    const MatX2& BP,
    const unsigned int num_samples )
{
    C.resize(num_samples,2);
    C.fill(0);
    VecX samples = uniformSampling(num_samples);
    unsigned int degree = BP.rows()-1;
    
    /* TODO : implement the De Casteljau's algorithm */

}

#endif