#include <TP6.h>
#include <SimpleViewer.h>
#include "todo.h"

// Eigen quick reference : http://eigen.tuxfamily.org/dox-devel/AsciiQuickReference.txt
#include <typedefs.h>

int main(int argc, char *argv[]) { 
    
    if(argc < 2) {
        std::cerr << "ERROR: input file required.\nusage: ./geonum_TP6 dataname\n";
        return -1;
    }
    const std::string dataname(argv[1]);
    const std::string in_dir = "../data/";
    const std::string in_filename = in_dir+dataname+".bpt";
    const std::string out_dir = "../plots/"+dataname+"/";
    
    // read Bezier net
    std::vector<MatdXX> netX, netY, netZ;
    if (!readBPT(in_filename,netX,netY,netZ))
    {
        std::cerr << "ERROR: failed to read " << in_filename << std::endl;
        return -1;
    }
    
    unsigned int num_samples = 20;

    // init the viewer
    SimpleViewer viewer;
    viewer.set_facecolor( 255, 163,   0 ); // [255,163,  0] = dark orange
    viewer.set_edgecolor(  55,  55,  55 ); // [ 55, 55, 55] = gray
    
    // compute Bezier patches
    for(unsigned int i=0; i < netX.size(); i++)
    {
        std::cout << "patch " << i+1 << "/" << netX.size() << std::endl;
        
        // extract control net for current patch
        MatdXX nX=netX[i], nY=netY[i], nZ=netZ[i];
        
        // compute current patch
        MatdXX X, Y, Z;
        BezierPatch(X,Y,Z,nX,nY,nZ,num_samples);
        
        // render in viewer
        viewer.add_patch(X,Y,Z);
        
        //
        // HINT :
        // if the SimpleViewer doesn't work for you, use the following code to export the computed patches
        // then render them using plot.py
        //
        // In that case you'll need to remove/comment #include <SimpleViewer.h> and the corresponding code in this file.
        //
        /*        
        std::string out_filename = out_dir+"patch"+std::to_string(1001+i)+".off";
        if (!writePatch(out_filename,X,Y,Z))
        {
            std::cerr << "ERROR: cannot write " << out_filename << "\n";
            return -1;
        }
        */
    }
    return viewer.show();
    return 0;
}