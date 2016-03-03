#ifndef TP4_GEONUM_TODO_H
#define TP4_GEONUM_TODO_H

void ChaikinStep(
    const MatX2& X0,
          MatX2& X1 )
{
    // TODO : implement one step of the Chaikin subdivision (for closed curves)
    //
    // Hint:
    // Given a sequence of N points x_i^k (i=0,...,N-1) on the k-th level of subdivision,
    // one step of the Chaikin's algorithm yields 2*N points x_j^{k+1} (j=0,...,2*N-1)
    // s.t.
    //
    // x_{2i  }^{k+1} = 0.75 * x_i^k + 0.25 * x_{i+1}^k
    // x_{2i+1}^{k+1} = 0.25 * x_i^k + 0.75 * x_{i+1}^k
    //
    unsigned int N = X0.rows();
    X1.resize(2*N,2);
    X1.fill(0);
    // TODO : compute X1 in a loop
    // Hint : Be careful with the indices!
    //        Use the modular arithmetic %N when needed.
}

void Chaikin(
    const MatX2& DataPoints,
    const unsigned int num_steps,
    const std::string out_dir ) 
{
    std::cout << "Chaikin..." << std::endl;
    
    // initialize the matrices X0, X1
    // X0 : points x_i^k    ,   N rows
    // X1 : points x_i^{k+1}, 2*N rows
    MatX2 X0=DataPoints, X1;
    
    // perform the algorithm num_steps times
    for(unsigned int s=1; s <= num_steps; ++s)
    {
        ChaikinStep(X0,X1);
        // Write s-th curve (now stored in X1) to file
        if( !writeData( out_dir+"curve"+std::to_string(s)+".txt", X1 )) {
            std::cerr << "ERROR: could not write the output file." << std::endl;
            return;
        }
        // Move to next iteration
        X0 = X1;
    }
    std::cout << "Chaikin done." << std::endl;
}

void CornerCutting(
    const MatX2& DataPoints,
    const double a,
    const double b,
    const unsigned int num_steps,
    const std::string out_dir )
{
    std::cout << "Corner cutting..." << std::endl;
    
    // TODO : implement the corner cutting subdivision (for closed curves) with parameters a, b
    //
    // Hint : This is almost the same as the implementation of Chaikin :
    //
    // x_{2i  }^{k+1} = (1-a) * x_i^k + a * x_{i+1}^k
    // x_{2i+1}^{k+1} = (1-b) * x_i^k + b * x_{i+1}^k
    //
    
    std::cout << "Corner cutting done." << std::endl;
}

void FourPoint(
    const MatX2& DataPoints,
    const unsigned int num_steps,
    const std::string out_dir ) 
{
    std::cout << "4 point..." << std::endl;
    
    // TODO : implement the 4 point subdivision scheme (for closed curves)
    //
    // Hint : Again, very similar to the previous methods.
    //
    // x_{2i  }^{k+1} = x_i^k
    // x_{2i+1}^{k+1} = 1.0/16.0 * (- x_{i-1}^k + 9*x_{i}^k + 9*x_{i+1}^k - x_{i+2}^k )
    //
    
    std::cout << "4 point done." << std::endl;
}

#endif