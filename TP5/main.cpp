#include "TP5.h"
#include "todo.h"

int main(int argc, char *argv[])
{
    if(argc < 2) {
        std::cerr << "ERROR: input file required.\nusage: ./geonum_TP5 simple\n";
        return -1;
    }

    const std::string dataname(argv[1]);
    const std::string in_filename = "../data/"+dataname+".data";
    const std::string out_filename = "../plots/"+dataname+".txt";
    
    // read data
    MatX2 DataPoints;
    if( !readData( in_filename, DataPoints )) {
        std::cerr << "ERROR: failed to open " << in_filename << std::endl;
        return -1;
    }
        
    MatX2 CurvePoints;
    const unsigned int
        degree    = 3, // degree of the curve
        num_steps = 5; // number of subdivision steps
        
    // compute uniform B-spline
    UniformBSpline( DataPoints, CurvePoints, degree, num_steps );
    
    // write data
    if( !writeData( out_filename, CurvePoints )) {
        std::cerr << "ERROR: could not write the output file." << std::endl;
        return -1;
    }   
    
    return 0;
}