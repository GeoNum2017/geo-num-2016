// standard headers
#include <TP7.h>
#include <SimpleViewer.h>
#include "todo.h"
// Eigen quick reference : http://eigen.tuxfamily.org/dox-devel/AsciiQuickReference.txt
#include <typedefs.h>

int main(int argc, char *argv[]) { 
    
    if(argc < 2) {
        std::cerr << "ERROR: input file required.\nusage: ./geonum_TP7 dataname\n";
        return -1;
    }

    const std::string dataname(argv[1]);
    const std::string in_dir = "../data/";
    const std::string in_filename = in_dir+dataname+".bspline";
    const std::string out_dir = "../plots/"+dataname+"/";
    
    // netX, netY, netZ : control net
    // X, Y, Z : surface points, all patches stored in one matrix per coordinate
    MatdXX netX, netY, netZ, X, Y, Z;
    
    // knot vectors in u, v directions
    VecdX knotsU, knotsV;
    
    // number of samples
    unsigned int num_samples = 10;
    
    // read the data from file
    readBSpline( in_filename, netX, netY, netZ, knotsU, knotsV );
    
    // evaluate B-spline points
    evaluateBSpline( X, Y, Z, netX, netY, netZ, knotsU, knotsV, num_samples );
    
    // init the viewer
    SimpleViewer viewer;
    // add our patch
    viewer.add_surface(X,Y,Z);
    //
    // TODO OPTIONAL : visualise the control net
    //viewer.add_surface(netX,netY,netZ);
    //
    
    viewer.set_facecolor( 255, 163,   0 ); // [255,163,  0] = dark orange
    viewer.set_edgecolor(  55,  55,  55 ); // [ 55, 55, 55] = gray
    return viewer.show();
    
    //
    // TODO OPTIONAL: do this if you still have problems with the compilation of SimpleViewer.
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
