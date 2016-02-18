#include "TP3.h"
#include "todo.h"

int main(int argc, char *argv[])
{
    // define number of samples to be computed
    const unsigned int num_samples = 50;   
        
    // read input data
    if(argc < 2) {
        std::cerr << "ERROR: input file required.\nusage: ./geonum_TP3 simple|spiral|circle|camel\n";
        return -1;
    }
    
    const std::string dataname(argv[1]);
    const std::string in_dir = "../data/";
    const std::string datatype = ".bspline";
    const std::string in_filename = in_dir + dataname + datatype;
    const std::string out_dir = "../plots/" + dataname + '/';
    
    MatX2 ControlPoints, SplinePoints;
    VecX Nodes;
    
    if(!readBSpline( in_filename, ControlPoints, Nodes )) {
        std::cerr << "ERROR: failed to open " << in_filename << std::endl;
        return -1;
    }
    evaluateBSpline( SplinePoints, ControlPoints, Nodes, num_samples );
    writeSplineData( SplinePoints, num_samples, out_dir );
    
    return 0;
}
