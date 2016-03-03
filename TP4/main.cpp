#include "TP4.h"
#include "todo.h"

int main(int argc, char *argv[])
{
    if(argc < 2) {
        std::cerr << "ERROR: input file required.\nusage: ./geonum_TP4 dataname";
        return -1;
    }    
    // Folders, filenames
    const std::string dataname(argv[1]);
    const std::string in_dir = "../data/";
    const std::string in_filename = in_dir + dataname+".data";
    const std::string out_dir = "../plots/"+dataname+'/';
    
    // Read data
    MatX2 DataPoints;
    if( !readData( in_filename, DataPoints )) {
        std::cerr << "ERROR: failed to open " << in_filename << std::endl;
        return -1;
    }
    
    // Set number of subdivision steps
    unsigned int num_steps = 5;
    
    // Uncomment one of the three subdivision schemes
    Chaikin( DataPoints, num_steps, out_dir );
    CornerCutting( DataPoints, 0.2, 0.8, num_steps, out_dir );
    FourPoint( DataPoints, num_steps, out_dir );
    
    return 0;
}
