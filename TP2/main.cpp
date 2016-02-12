#include "TP2.h"
#include "todo.h"

int main(int argc, char *argv[])
{
    // define number of samples to be computed
    const unsigned int num_samples = 50;   
        
    // read Bezier points
    if(argc < 2) {
        std::cerr << "ERROR: input file required.\nusage: ./geonum_TP2 simple|spiral|semi|infinity" << std::endl;
        return -1;
    }
    
    const std::string dataname(argv[1]);
    const std::string in_filename = "../data/"+dataname+".bcv";
    const std::string out_dir = "../plots/" + dataname + "/";
    
    MatX2 InputPoints;
    if(!readBCV(in_filename,InputPoints)) {
        std::cerr << "ERROR: failed to open " << in_filename << std::endl;
        return -1;
    }
    const unsigned int curve_count = InputPoints.rows()-1;
    unsigned int  degree;
    
    //
    // Spline points are exported by curves, as ./plots/%dataname%/c%k%/curve%i%.txt
    //
    
    // C1 quadratic spline
    degree = 2;
    MatX2 ControlPointsC1, SplinePointsC1;
    computeSplineC1( ControlPointsC1, InputPoints );
    if( !evaluateSpline( SplinePointsC1, ControlPointsC1, degree, num_samples ))
        return -1;
    if( !writeSplineData( SplinePointsC1, curve_count, num_samples, out_dir+"c1/" ))
        return -1;
    
    // C2 cubic spline
    degree = 3;
    MatX2 ControlPointsC2, SplinePointsC2;
    computeSplineC2( ControlPointsC2, InputPoints );
    if( !evaluateSpline( SplinePointsC2, ControlPointsC2, degree, num_samples ))
        return -1;
    if( !writeSplineData( SplinePointsC2, curve_count, num_samples, out_dir+"c2/" ))
        return -1;

    return 0;
}
