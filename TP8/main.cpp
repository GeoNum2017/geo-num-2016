// standard headers
#include <TP8.h>
#include <SimpleViewer.h>
#include "todo.h"
// Eigen quick reference : http://eigen.tuxfamily.org/dox-devel/AsciiQuickReference.txt
#include <typedefs.h>

int main(int argc, char *argv[]) { 
    
    if(argc < 2) {
        std::cerr << "ERROR: input file required.\nusage: ./geonum_TP8 dataname\n";
        return -1;
    }

    const std::string dataname(argv[1]);
    const std::string in_dir = "../data/";
    const std::string in_filename = in_dir+dataname+".net";
    const std::string out_dir = "../plots/"+dataname+"/";
    
    // netX, netY, netZ : control net
    // X, Y, Z : surface points, all patches stored in one matrix per coordinate
    MatdXX netX, netY, netZ, X, Y, Z;
    bool u_closed, v_closed;
        
    // read the data from file
    if( !readControlPoints( in_filename, netX, netY, netZ, u_closed, v_closed )) {
        std::cerr << "ERROR: cannot read data '" << dataname << "'" << std::endl;
        std::cerr << "should be one of { grid, cylinder, torus }" << std::endl;
        return -2;
    }
    
    // number of subdivision steps
    // start with 1, then try 2, 3, 4, 5
    unsigned int num_steps = 1;
    
    // subdivide
    subdivide( X, Y, Z, netX, netY, netZ, u_closed, v_closed, num_steps );
    
    //*******************//
    //** SimpleViewer **//
    //*****************//
    // init the viewer
    SimpleViewer viewer;
    
    // add our patch
    viewer.add_surface(X,Y,Z,u_closed,v_closed);
    
    // TODO OPTIONAL : visualise the control net
    //viewer.add_surface(netX,netY,netZ);
    
    viewer.set_facecolor( 255, 163,   0 ); // [255,163,  0] = dark orange
    viewer.set_edgecolor(  55,  55,  55 ); // [ 55, 55, 55] = gray
    return viewer.show();
    
    //*******************************//
    //** OPTIONAL : Export to OFF **//
    //*****************************//
    //
    // OPTIONAL TODO : do this if you still have problems with the compilation of SimpleViewer.
    //
    // If the SimpleViewer doesn't work for you, use the following code to export the computed patches.
    // Then render them using plot.py as usual; the script is provided.
    //
    // In this case, you'll need to remove/comment #include <SimpleViewer.h> and the corresponding viewer lines in this file.
    //
    
    /*
    std::string out_filename = out_dir+"patch.off";
    if (!writePatch(out_filename,X,Y,Z))
    {
        std::cerr << "ERROR: cannot write " << out_filename << "\n";
        return -1;
    }
    else
        std::cerr << "  " << out_filename << " exported. (" << X.size() << " vertices)\n";
    */
}
