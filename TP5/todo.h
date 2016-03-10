#ifndef TP5_GEONUM_TODO_H
#define TP5_GEONUM_TODO_H

void UniformBSpline(
    const MatX2& DataPoints,
          MatX2& Curve,
    const unsigned int degree,
    const unsigned int num_steps) 
{
    // TODO : implement uniform B-spline curve subdivision scheme
    // one subdivision step :
    // 1. double the points
    // 2. average k times
    // 3. replace
}

#endif