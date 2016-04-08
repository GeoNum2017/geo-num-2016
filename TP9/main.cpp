#include <TP9.h>
#include <SimpleViewer.h>
#include <typedefs.h>
#include <ctime>
#include "todo.h"
/* Eigen quick reference : http://eigen.tuxfamily.org/dox-devel/AsciiQuickReference.txt */

std::clock_t begin, end;

int main(int argc, char *argv[]) { 
    
    if(argc < 2) {
        std::cerr << "ERROR: input file required.\nusage: ./geonum_TP9 [dataname] [numsteps=1]\n";
        return -1;
    }

    const std::string dataname(argv[1]);
    const std::string in_dir = "../data/";
    const std::string in_filename = in_dir+dataname+".off";
    const std::string out_dir = "../plots/";
    const std::string out_filename = out_dir+dataname+".off";
    
    // mesh data : vertices and faces
    MatdX3 V0, V1;
    MatiX3 F0, F1;
        
    // read mesh from OFF file
    if( !readOFF( in_filename, V0, F0 )) {
        std::cerr << "ERROR: cannot read file " << in_filename << std::endl;
        return -1;
    }
    
    unsigned int num_steps = 1;
    if( argc > 2 )
        num_steps = atoi(argv[2]);
    
    for( int s=1; s<=num_steps; s++ ){
        std::cout << "\nLoop step " << s << "...";
        assert(V0.rows() < 20000); // limit
        begin = std::clock();
        LoopStep(V0,F0,V1,F1);
        end = std::clock();
        V0 = V1;
        F0 = F1;
        std::cout << 
            " [" << double(end - begin) / CLOCKS_PER_SEC << "s]\n"
            "  #V" << s << " = " << V0.rows() << std::endl << 
            "  #F" << s << " = " << F0.rows() << std::endl;
    }
    std::cout << "\n" << std::endl;
    
    // write mesh to OFF file
    if( !writeOFF( out_filename, V0, F0 )) {
        std::cerr << "ERROR: cannot write file " << out_filename << std::endl;
        return -1;
    }
    
    //*******************//
    //** SimpleViewer **//
    //*****************//
    SimpleViewer viewer;
    viewer.set_mesh(V0,F0);    
    viewer.set_facecolor( 255, 163,   0 ); // [255,163,  0] = dark orange
    viewer.set_edgecolor(  55,  55,  55 ); // [ 55, 55, 55] = gray
    return viewer.show();
}
