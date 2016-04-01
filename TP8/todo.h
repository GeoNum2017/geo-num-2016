#ifndef TP8_GEONUM_TODO_H
#define TP8_GEONUM_TODO_H

/***************************************************/
/* subdivideStep() - perform one subdivision step */
/*************************************************/
void subdivideStep(
    const MatdXX& X0,
    const MatdXX& Y0,
    const MatdXX& Z0,
          MatdXX& X1,
          MatdXX& Y1,
          MatdXX& Z1,
          bool u_closed,
          bool v_closed )
{   
    // TODO : comment this when appropriate
    X1 = X0; Y1 = Y0; Z1 = Z0;
    
    
    unsigned int
        u0 = X0.cols(),
        v0 = X0.rows(), // number of vertices in both directions, current iteration (X0, Y0, Z0)
        u1, v1;         // number of vertices in both directions, next iteration (X1, Y1, Z1)
    
    //
    // TODO : implement one subdivision step
    //
    // treat all of the following data in your implementation
    //  - surface open in both directions       ( u_closed==0, v_closed==0 )  [ grid     ]
    //  - surface closed in one direction only  ( u_closed==1, v_closed==0 ) 
    //                                      or  ( u_closed==0, v_closed==1 )  [ cylinder ]
    //  - surface closed in both directions     ( u_closed==1, v_closed==1 )  [ torus    ]
    //
    
    // TODO : set u1 and v1 to their right values, depending on the values of u_closed and v_closed
    // u1 =
    // v1 = 
    
    // TODO : init as zeros, uncomment when needed
    //X1.setZero(u1,v1);Y1.setZero(u1,v1);Z1.setZero(u1,v1);
}


/******************************************************/
/* subdivide() - perform n subdivision steps */
/****************************************************/
void subdivide(
          MatdXX& X,
          MatdXX& Y,
          MatdXX& Z,
    const MatdXX& netX,
    const MatdXX& netY,
    const MatdXX& netZ,
          bool u_closed,
          bool v_closed,
          unsigned int n )
{
    // initialize
    MatdXX 
        X0=netX, 
        Y0=netY, 
        Z0=netZ;
    for( unsigned s=0; s<n; s++ ) {
        // do one step
        subdivideStep( X0, Y0, Z0, X, Y, Z, u_closed, v_closed );
        // iterate
        X0 = X;
        Y0 = Y;
        Z0 = Z;
    }
    // ... surface subdivided num_step times is stored in X, Y, Z
}

#endif
