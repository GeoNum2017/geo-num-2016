#include <TP7.h>
#include <SimpleViewer.h>
#include <typedefs.h>
#include "todo.h"
// Eigen quick reference : http://eigen.tuxfamily.org/dox-devel/AsciiQuickReference.txt

int main(int argc, char *argv[]) { 
    
    if(argc < 2) {
        std::cerr << "ERROR: input file required.\nusage: ./geonum_TP7 dataname\n";
        return -1;
    }

    const std::string dataname(argv[1]);
    const std::string in_dir = "../data/";
    const std::string in_filename = in_dir+dataname+".bspline";
    const std::string out_filename = "../plots/"+dataname+".off";
    
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
    

    //*****************//
    //** Export OFF **//
    //***************//
    if (!writePatch(out_filename,X,Y,Z))
    {
        std::cerr << "ERROR: cannot write " << out_filename << "\n";
        return -1;
    }
    else
        std::cerr << "  " << out_filename << " exported. (" << X.size() << " vertices)\n";
    
    
    //*******************//
    //** SimpleViewer **//
    //*****************//
    SimpleViewer viewer;
    // add our patch
    // TODO : uncomment this after implementing evaluateBSpline()
    //viewer.add_surface(X,Y,Z);
    
    // visualise the control net
    viewer.add_wireframe(netX,netY,netZ);
    
    viewer.set_facecolor( 255, 163,   0 ); // [255,163,  0] = dark orange
    viewer.set_edgecolor(  55,  55,  55 ); // [ 55, 55, 55] = gray
    return viewer.show();
}
