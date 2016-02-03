#include "TP1.h"
#include "todo.h"

int main()
{
    // define number of samples to be computed
    unsigned int num_samples = 100;    
    
    // Bezier points = control polygon, (n+1)x2 matrix
    MatX2 BP;
    
    // curve points
    MatX2 C_Casteljau, C_Bernstein;
    
    // read Bezier points
    if( !readBCV("../data/simple.bcv",BP) ) return -1;
    
    // compute curve points using Bernstein basis
    std::clock_t start = std::clock();
    BernsteinCurve( C_Bernstein, BP, num_samples );
    measureTime(start);
    
    // compute curve points using DeCasteljau
    start = std::clock();
    CasteljauCurve( C_Casteljau, BP, num_samples );
    measureTime(start);
    
    // export curves
    if( !writeMatrixData("../plots/output.txt",C_Bernstein) ) return -1;

    return 0;
}
