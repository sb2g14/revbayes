#include "RateMatrix_PoMoBalance4N.h"

#include <assert.h>
#include <cstddef>

#include "RbException.h"
#include "Assignable.h"
#include "Cloneable.h"
#include "MatrixReal.h"
#include "TransitionProbabilityMatrix.h"
#include "RbVector.h"
#include "RbVectorImpl.h"

using namespace RevBayesCore;

/** Construct rate matrix with n states, an exchangeability matrix, a simplex of equilibrium frequencies, and a virtual population size */
RateMatrix_PoMoBalance4N::RateMatrix_PoMoBalance4N( long num_states, long in_n )  :
AbstractRateMatrix( num_states ), 
N( in_n ),
mu( 12 , 0.01 ),
phi( 4 , 1.0 ),
beta( 6, 1.0 ),
B( 6, 1 )
{
  update();
}

/** Copy constructor */
RateMatrix_PoMoBalance4N::RateMatrix_PoMoBalance4N(const RateMatrix_PoMoBalance4N& m) :
AbstractRateMatrix( m ), 
N( m.N ),
mu( m.mu ),
phi( m.phi ),
beta( m.beta ),
B( m.B )
{

}


RateMatrix_PoMoBalance4N& RateMatrix_PoMoBalance4N::operator=(const RateMatrix_PoMoBalance4N &r)
{

  if (this != &r)
  {
    AbstractRateMatrix::operator=( r );
    N                   = r.N;
    mu                  = r.mu;
    phi                 = r.phi;
    beta                = r.beta;
    B                   = r.B;
  }

  return *this;
}




/** Destructor */
RateMatrix_PoMoBalance4N::~RateMatrix_PoMoBalance4N(void)
{

}



RateMatrix_PoMoBalance4N& RateMatrix_PoMoBalance4N::assign(const Assignable &m)
{

    const RateMatrix_PoMoBalance4N *rm = dynamic_cast<const RateMatrix_PoMoBalance4N*>(&m);
    if ( rm != NULL )
    {
        return operator=(*rm);
    }
    else
    {
        throw RbException("Could not assign rate matrix.");
    }
}


double RateMatrix_PoMoBalance4N::averageRate(void) const
{
    return 1.0;
}





void RateMatrix_PoMoBalance4N::buildRateMatrix(void)
{

 /*  
  INFORMATION ABOUT THE PoMoBalance4N RATE MATRIX

  It includes both fixed and polymorphic sites, but only biallelic states are considered.

  The pomo rate matrices defined here first list the fixed states {Na0}, {Na1} ...,
  these occupying positions 0:(K-1), and then polymorphic states.

  K alleles comprise (K*K-K)/2 pairwise combinations of alleles.
  This is the number of edges in the pomo state-space. Each edge comprises N-1 polymorphic states, 
  each of which represents a state of allelic frequencies in the population (summing to N).
  Example for a random allele pair aiaj: {(N-1)ai,1aj}, {(N-2)ai,2aj}, ..., {1ai,(N-1)aj}

  The polymorphic edges are listed in the following order a0a1, a0a2, a0a3, ..., a(K-2)aK-1
  We say the a0a1 polymorphic states sit at edge 0. Thus, {(N-1)a0,1a1} sits at position K and 
  {1a0,(N-1)a1} sits N-2 positions further (i.e., K+N-2).
  More generally, the polymorphic states of the allele pair sitting at edge E occupy the positions
  [K+E*N-E]:[K+(E+1)*(N-1)-1].

  Additionally to the previous models, the balancing selection multiplier is added
  phi[i]*pow(beta[j],0.5*(abs(N-n-B[j])-abs(N-n-1-B[j])+1.0)
  */

   MatrixReal& m = *the_rate_matrix;

  //populate rate matrix with 0.0
  // **total waste of time with sparse matrices like pomos**
  for (int i=0; i< num_states; i++){
    for (int j=0; j< num_states; j++){
      m[i][j] = 0.0;        
    }
  }


  // set the diagonal vector
  std::vector< double > diagonal(num_states,0.0); 


//  //mutations
//  //AC
//  m[0][4]      = N*mu[0];    //{NA} -> {(N-1)A,1C}
//  m[1][N+2]    = N*mu[1];    //{NC} -> {1A,(N-1)C}
//
//  //AG
//  m[0][N+3]    = N*mu[2];    //{NA} -> {(N-1)A,1G}
//  m[2][2*N+1]  = N*mu[3];    //{NG} -> {1A,(N-1)G}
//
//  //AT
//  m[0][2*N+2]  = N*mu[4];    //{NA} -> {(N-1)A,1T}
//  m[3][3*N]    = N*mu[5];    //{NT} -> {1A,(N-1)T}
//
//  //CG
//  m[1][3*N+1]  = N*mu[6];    //{NC} -> {(N-1)C,1aG}
//  m[2][4*N-1]  = N*mu[7];    //{NG} -> {1C,(N-1)aG}
//
//  //CT
//  m[1][4*N]    = N*mu[8];    //{NC} -> {(N-1)C,1T}
//  m[3][5*N-2]  = N*mu[9];    //{NT} -> {1C,(N-1)T}
//
//  //GT
//  m[2][5*N-1]  = N*mu[10];    //{NG} -> {(N-1)G,1T}
//  m[3][6*N-3]  = N*mu[11];    //{NT} -> {1G,(N-1)T}
//
//  diagonal[0] =  m[0][4]     + m[0][N+3]   + m[0][2*N+2];
//  diagonal[1] =  m[1][N+2]   + m[1][3*N+1] + m[1][4*N];
//  diagonal[2] =  m[2][2*N+1] + m[2][4*N-1] + m[2][5*N-1];
//  diagonal[3] =  m[3][3*N]   + m[3][5*N-2] + m[3][6*N-3];
//
//  //fixations
//  if (N == 2) {
//      //AC
//      m[4][0] = (N - 1.0) * phi[0] / ((N - 1) * phi[0] + phi[1]);  //{(N-1)A,1C} -> {NA}
//      m[N + 2][1] = (N - 1.0) * phi[1] / ((N - 1) * phi[1] + phi[0]);  //{1A,(N-1)C} -> {NC}
//      diagonal[4] += m[4][0];
//      diagonal[N + 2] += m[N + 2][1];
//
//      //AG
//      m[N + 3][0] = (N - 1.0) * phi[0] / ((N - 1) * phi[0] + phi[2]);  //{(N-1)A,1G} -> {NA}
//      m[2 * N + 1][2] = (N - 1.0) * phi[2] / ((N - 1) * phi[2] + phi[0]);  //{1A,(N-1)G} -> {NG}
//      diagonal[N + 3] += m[N + 3][0];
//      diagonal[2 * N + 1] += m[2 * N + 1][2];
//
//      //AT
//      m[2 * N + 2][0] = (N - 1.0) * phi[0] / ((N - 1) * phi[0] + phi[3]);  //{(N-1)A,1T} -> {NA}
//      m[3 * N][3] = (N - 1.0) * phi[3] / ((N - 1) * phi[3] + phi[0]);  //{1A,(N-1)T} -> {NT}
//      diagonal[2 * N + 2] += m[2 * N + 2][0];
//      diagonal[3 * N] += m[3 * N][3];
//
//      //CG
//      m[3 * N + 1][1] = (N - 1.0) * phi[1] / ((N - 1) * phi[1] + phi[2]);  //{(N-1)C,1G} -> {NC}
//      m[4 * N - 1][2] = (N - 1.0) * phi[2] / ((N - 1) * phi[2] + phi[1]);  //{1C,(N-1)G} -> {NG}
//      diagonal[3 * N + 1] += m[3 * N + 1][1];
//      diagonal[4 * N - 1] += m[4 * N - 1][2];
//
//      //CT
//      m[4 * N][1] = (N - 1.0) * phi[1] / ((N - 1) * phi[1] + phi[3]);  //{(N-1)C,1T} -> {NC}
//      m[5 * N - 2][3] = (N - 1.0) * phi[3] / ((N - 1) * phi[3] + phi[1]);  //{1C,(N-1)T} -> {NT}
//      diagonal[4 * N] += m[4 * N][1];
//      diagonal[5 * N - 2] += m[5 * N - 2][3];
//
//      //GT
//      m[5 * N - 1][2] = (N - 1.0) * phi[2] / ((N - 1) * phi[2] + phi[3]);  //{(N-1)G,1T} -> {NG}
//      m[6 * N - 3][3] = (N - 1.0) * phi[3] / ((N - 1) * phi[3] + phi[2]);  //{1G,(N-1)T} -> {NT}
//      diagonal[5 * N - 1] += m[5 * N - 1][2];
//      diagonal[6 * N - 3] += m[6 * N - 3][3];
//  }
//
//  //reciprocal of the population size
//  // double rN = 1.0/N; // Change to the probability normalisation later on
//
//  // For speeding up the calculations
//  double a1 = 0.0;
//  double a2 = 0.0;
//
//  //the pomo rate matrix is entirely defined by fixations and mutations if N=2
//  if (N>2) {
//
//    //frequency shifts from singletons
//    // Beginning of the chain
//    //AC
//    // n = 1
//    m[N + 2][1] = (N - 1.0) * phi[1] / (phi[1] + (N-1.0)*phi[0]*pow(beta[0],0.5*(abs(  1-B[0])-abs(  2-B[0])+1.0)));  //{1A,(N-1)C} -> {NC}
//    //m[N+2]   [N+1]   = (N-1.0)*phi[0]*pow(beta[0],0.5*(abs(  1-B[0])-abs(  2-B[0])+1.0)) / (phi[1] + (N-1.0)*phi[0]*pow(beta[0],0.5*(abs(  1-B[0])-abs(  2-B[0])+1.0)));  //{1A,(N-1)C} -> {2A,(N-2)C}
//    a1 = (N-1.0)*phi[0]*pow(beta[0],0.5*(abs(  1-B[0])-abs(  2-B[0])+1.0));
//    m[N+2]   [N+1] = a1 / (phi[1] + a1);
//    diagonal[N+2] += m[N+2][N+1];
//    diagonal[N + 2] += m[N + 2][1];
//
//    //AG
//    // n = 1
//    m[2 * N + 1][2] = (N - 1.0) * phi[2] / (phi[2] + (N-1.0)*phi[0]*pow(beta[1],0.5*(abs(  1-B[1])-abs(  2-B[1])+1.0)));  //{1A,(N-1)G} -> {NG}
//    //m[2*N+1] [2*N]   = (N-1.0)*phi[0]*pow(beta[1],0.5*(abs(  1-B[1])-abs(  2-B[1])+1.0)) / (phi[2] + (N-1.0)*phi[0]*pow(beta[1],0.5*(abs(  1-B[1])-abs(  2-B[1])+1.0)));  //{1A,(N-1)G} -> {2A,(N-2)G}
//    a1 = (N-1.0)*phi[0]*pow(beta[1],0.5*(abs(  1-B[1])-abs(  2-B[1])+1.0));
//    m[2*N+1] [2*N] = a1 / (phi[2] + a1);
//    diagonal[2 * N + 1] += m[2 * N + 1][2];
//    diagonal[2*N+1] += m[2*N+1][2*N];
//
//    //AT
//    // n = 1
//    m[3 * N][3] = (N - 1.0) * phi[3] / (phi[3] + (N-1.0)*phi[0]*pow(beta[2],0.5*(abs(  1-B[2])-abs(  2-B[2])+1.0)));  //{1A,(N-1)T} -> {NT}
//    //m[3*N]   [3*N-1] = (N-1.0)*phi[0]*pow(beta[2],0.5*(abs(  1-B[2])-abs(  2-B[2])+1.0)) / (phi[3] + (N-1.0)*phi[0]*pow(beta[2],0.5*(abs(  1-B[2])-abs(  2-B[2])+1.0)));  //{1A,(N-1)T} -> {2A,(N-2)T}
//    a1 = (N-1.0)*phi[0]*pow(beta[2],0.5*(abs(  1-B[2])-abs(  2-B[2])+1.0));
//    m[3*N]   [3*N-1] = a1 / (phi[3] + a1);
//    diagonal[3 * N] += m[3 * N][3];
//    diagonal[3*N]   += m[3*N][3*N-1];
//
//    //CG
//    // n = 1
//    m[4 * N - 1][2] = (N - 1.0) * phi[2] / (phi[2] + (N-1.0)*phi[1]*pow(beta[3],0.5*(abs(  1-B[3])-abs(  2-B[3])+1.0)));  //{1C,(N-1)G} -> {NG}
//    //m[4*N-1] [4*N-2] = (N-1.0)*phi[1]*pow(beta[3],0.5*(abs(  1-B[3])-abs(  2-B[3])+1.0)) / (phi[2] + (N-1.0)*phi[1]*pow(beta[3],0.5*(abs(  1-B[3])-abs(  2-B[3])+1.0)));  //{1C,(N-1)G} -> {2C,(N-2)G}
//    a1 = (N-1.0)*phi[1]*pow(beta[3],0.5*(abs(  1-B[3])-abs(  2-B[3])+1.0));
//    m[4*N-1] [4*N-2] = a1 / (phi[2] + a1);
//    diagonal[4 * N - 1] += m[4 * N - 1][2];
//    diagonal[4*N-1] += m[4*N-1][4*N-2];
//
//    //CT
//    // n = 1
//    m[5 * N - 2][3] = (N - 1.0) * phi[3] / (phi[3] + (N-1.0)*phi[1]*pow(beta[4],0.5*(abs(  1-B[4])-abs(  2-B[4])+1.0)));  //{1C,(N-1)T} -> {NT}
//    //m[5*N-2] [5*N-3] = (N-1.0)*phi[1]*pow(beta[4],0.5*(abs(  1-B[4])-abs(  2-B[4])+1.0)) / (phi[3] + (N-1.0)*phi[1]*pow(beta[4],0.5*(abs(  1-B[4])-abs(  2-B[4])+1.0)));  //{1C,(N-1)T} -> {2C,(N-2)T}
//    a1 = (N-1.0)*phi[1]*pow(beta[4],0.5*(abs(  1-B[4])-abs(  2-B[4])+1.0));
//    m[5*N-2] [5*N-3] = a1 / (phi[3] + a1);
//    diagonal[5 * N - 2] += m[5 * N - 2][3];
//    diagonal[5*N-2] += m[5*N-2][5*N-3];
//
//    //GT
//    // n = 1
//    m[6 * N - 3][3] = (N - 1.0) * phi[3] / (phi[3] + (N-1.0)*phi[2]*pow(beta[5],0.5*(abs(  1-B[5])-abs(  2-B[5])+1.0)));  //{1G,(N-1)T} -> {NT}
//    //m[6*N-3] [6*N-4] = (N-1.0)*phi[2]*pow(beta[5],0.5*(abs(  1-B[5])-abs(  2-B[5])+1.0)) / (phi[3] + (N-1.0)*phi[2]*pow(beta[5],0.5*(abs(  1-B[5])-abs(  2-B[5])+1.0)));  //{1G,(N-1)T} -> {2G,(N-2)T}
//    a1 = (N-1.0)*phi[2]*pow(beta[5],0.5*(abs(  1-B[5])-abs(  2-B[5])+1.0));
//    m[6*N-3] [6*N-4] = a1 / (phi[3] + a1);
//    diagonal[6 * N - 3] += m[6 * N - 3][3];
//    diagonal[6*N-3] += m[6*N-3][6*N-4];
//
//    // End of the chain
//    //AC
//    // n = N - 1
//    //m[4]     [5]     = (N - 1.0)*phi[1]*pow(beta[0],0.5*(abs(N-1-B[0])-abs(N-2-B[0])+1.0))/((N-1.0)*phi[1]*pow(beta[0],0.5*(abs(N-1-B[0])-abs(N-2-B[0])+1.0)) + phi[0]);  //{(N-1)A,1C} -> {(N-2)A,2C}
//    a1 = (N - 1.0)*phi[1]*pow(beta[0],0.5*(abs(N-1-B[0])-abs(N-2-B[0])+1.0));
//    m[4]     [5]     = a1 / (a1 + phi[0]);
//    m[4][0] = (N - 1.0) * phi[0] / ((N-1)*phi[1]*pow(beta[0],0.5*(abs(N-1-B[0])-abs(N-2-B[0])+1.0)) + phi[0]);  //{(N-1)A,1C} -> {NA}
//    diagonal[4]   += m[4][5];
//    diagonal[4] += m[4][0];
//
//    //AG
//    // n = N - 1
//    //m[N+3]   [N+4]   = (N-1.0)*phi[2]*pow(beta[1],0.5*(abs(N-1-B[1])-abs(N-2-B[1])+1.0))/((N-1.0)*phi[2]*pow(beta[1],0.5*(abs(N-1-B[1])-abs(N-2-B[1])+1.0))+phi[0]);  //{(N-1)A,1G} -> {(N-2)A,2G}
//    a1 = (N-1.0)*phi[2]*pow(beta[1],0.5*(abs(N-1-B[1])-abs(N-2-B[1])+1.0));
//    m[N+3]   [N+4]   = a1 / (a1 + phi[0]);
//    m[N + 3][0] = (N - 1.0) * phi[0]/((N-1.0)*phi[2]*pow(beta[1],0.5*(abs(N-1-B[1])-abs(N-2-B[1])+1.0))+phi[0]);  //{(N-1)A,1G} -> {NA}
//    diagonal[N+3]   += m[N+3][N+4];
//    diagonal[N + 3] += m[N + 3][0];
//
//    //AT
//    // n = N - 1
//    //m[2*N+2] [2*N+3] = (N-1.0)*phi[3]*pow(beta[2],0.5*(abs(N-1-B[2])-abs(N-2-B[2])+1.0)) / ((N - 1)*phi[3]*pow(beta[2],0.5*(abs(N-1-B[2])-abs(N-2-B[2])+1.0)) + phi[0]);  //{(N-1)A,1T} -> {(N-2)A,2T}
//    a1 = (N-1.0)*phi[3]*pow(beta[2],0.5*(abs(N-1-B[2])-abs(N-2-B[2])+1.0));
//    m[2*N+2] [2*N+3] = a1 / (a1 + phi[0]);
//    m[2 * N + 2][0] = (N - 1.0) * phi[0] / ((N - 1) * phi[3]*pow(beta[2],0.5*(abs(N-1-B[2])-abs(N-2-B[2])+1.0)) + phi[0]);  //{(N-1)A,1T} -> {NA}
//    diagonal[2 * N + 2] += m[2 * N + 2][0];
//    diagonal[2*N+2] += m[2*N+2][2*N+3];
//
//    //CG
//    // n = N - 1
//    //m[3*N+1] [3*N+2] = (N-1.0)*phi[2]*pow(beta[3],0.5*(abs(N-1-B[3])-abs(N-2-B[3])+1.0))/((N-1.0)*phi[2]*pow(beta[3],0.5*(abs(N-1-B[3])-abs(N-2-B[3])+1.0)) + phi[1]);   //{(N-1)C,1G} -> {(N-2)C,2G}
//    a1 = (N-1.0)*phi[2]*pow(beta[3],0.5*(abs(N-1-B[3])-abs(N-2-B[3])+1.0));
//    m[3*N+1] [3*N+2] = a1 / (a1 + phi[1]);
//    m[3 * N + 1][1] = (N - 1.0) * phi[1] / ((N-1.0)*phi[2]*pow(beta[3],0.5*(abs(N-1-B[3])-abs(N-2-B[3])+1.0)) + phi[1]);  //{(N-1)C,1G} -> {NC}
//    diagonal[3 * N + 1] += m[3 * N + 1][1];
//    diagonal[3*N+1] += m[3*N+1][3*N+2];
//
//    //CT
//    // n = N - 1
//    //m[4*N]   [4*N+1] = (N-1.0)*phi[3]*pow(beta[4],0.5*(abs(N-1-B[4])-abs(N-2-B[4])+1.0))/((N-1.0)*phi[3]*pow(beta[4],0.5*(abs(N-1-B[4])-abs(N-2-B[4])+1.0)) + phi[1]);  //{(N-1)C,1T} -> {(N-2)C,2T}
//    a1 = (N-1.0)*phi[3]*pow(beta[4],0.5*(abs(N-1-B[4])-abs(N-2-B[4])+1.0));
//    m[4*N]   [4*N+1] = a1 / (a1 + phi[1]);
//    m[4 * N][1] = (N - 1.0) * phi[1]/((N-1.0)*phi[3]*pow(beta[4],0.5*(abs(N-1-B[4])-abs(N-2-B[4])+1.0)) + phi[1]);  //{(N-1)C,1T} -> {NC}
//    diagonal[4 * N] += m[4 * N][1];
//    diagonal[4*N]   += m[4*N][4*N+1];
//
//    //GT
//    // n = N - 1
//    //m[5*N-1] [5*N]   = (N-1.0)*phi[3]*pow(beta[5],0.5*(abs(N-1-B[5])-abs(N-2-B[5])+1.0))/((N-1.0)*phi[3]*pow(beta[5],0.5*(abs(N-1-B[5])-abs(N-2-B[5])+1.0)) + phi[2]);  //{(N-1)G,1T} -> {(N-2)G,2T}
//    a1 = (N-1.0)*phi[3]*pow(beta[5],0.5*(abs(N-1-B[5])-abs(N-2-B[5])+1.0));
//    m[5*N-1] [5*N]   = a1 / (a1 + phi[2]);
//    m[5 * N - 1][2] = (N - 1.0) * phi[2]/((N-1.0)*phi[3]*pow(beta[5],0.5*(abs(N-1-B[5])-abs(N-2-B[5])+1.0)) + phi[2]);  //{(N-1)G,1T} -> {NG}
//    diagonal[5 * N - 1] += m[5 * N - 1][2];
//    diagonal[5*N-1] += m[5*N-1][5*N];
//
//
//      //frequency shifts for all the other polymorphic states
//    if (N>3) {
//
//      for (int n=2; n<(N-1); n++){
//
//        //AC
//        a1 = phi[1]*pow(beta[0],0.5*(abs(N-n-B[0])-abs(N-n-1-B[0])+1.0));
//        a2 = phi[0]*pow(beta[0],0.5*(abs(N-n-B[0])-abs(N-n+1-B[0])+1.0));
//        //m[n+3]     [n+4]      = n*(N-n)*phi[1]*pow(beta[0],0.5*(abs(N-n-B[0])-abs(N-n-1-B[0])+1.0))/(n*phi[1]*pow(beta[0],0.5*(abs(N-n-B[0])-abs(N-n-1-B[0])+1.0))+(N-n)*phi[0]*pow(beta[0],0.5*(abs(N-n-B[0])-abs(N-n+1-B[0])+1.0))); //{(N-n)A,nC} -> {(N-n-1)A,(n+1)C}
//        m[n+3]     [n+4]      = n*(N-n) * a1 / (n*a1 + (N-n)*a2);
//        //m[n+3]     [n+2]      = n*(N-n)*phi[0]*pow(beta[0],0.5*(abs(N-n-B[0])-abs(N-n+1-B[0])+1.0))/(n*phi[1]*pow(beta[0],0.5*(abs(N-n-B[0])-abs(N-n-1-B[0])+1.0))+(N-n)*phi[0]*pow(beta[0],0.5*(abs(N-n-B[0])-abs(N-n+1-B[0])+1.0))); //{(N-n)A,nC} -> {(N-n+1)A,(n-1)C}
//        m[n+3]     [n+2]      = n*(N-n) * a2 / (n*a1 + (N-n)*a2);
//        diagonal[n+3] += m[n+3][n+4] + m[n+3][n+2];
//
//        //AG
//        a1 = phi[2]*pow(beta[1],0.5*(abs(N-n-B[1])-abs(N-n-1-B[1])+1.0));
//        a2 = phi[0]*pow(beta[1],0.5*(abs(N-n-B[1])-abs(N-n+1-B[1])+1.0));
//        //m[N+n+2]   [N+n+3]    = n*(N-n)*phi[2]*pow(beta[1],0.5*(abs(N-n-B[1])-abs(N-n-1-B[1])+1.0))/(n*phi[2]*pow(beta[1],0.5*(abs(N-n-B[1])-abs(N-n-1-B[1])+1.0))+(N-n)*phi[0]*pow(beta[1],0.5*(abs(N-n-B[1])-abs(N-n+1-B[1])+1.0))); //{(N-n)A,nG} -> {(N-n-1)A,(n+1)G}
//        m[N+n+2]   [N+n+3]    = n*(N-n) * a1 / (n*a1 + (N-n)*a2);
//        //m[N+n+2]   [N+n+1]    = n*(N-n)*phi[0]*pow(beta[1],0.5*(abs(N-n-B[1])-abs(N-n+1-B[1])+1.0))/(n*phi[2]*pow(beta[1],0.5*(abs(N-n-B[1])-abs(N-n-1-B[1])+1.0))+(N-n)*phi[0]*pow(beta[1],0.5*(abs(N-n-B[1])-abs(N-n+1-B[1])+1.0))); //{(N-n)A,nG} -> {(N-n+1)A,(n-1)G}
//        m[N+n+2]   [N+n+1]    = n*(N-n) * a2 / (n*a1 + (N-n)*a2);
//        diagonal[N+n+2] += m[N+n+2][N+n+3] + m[N+n+2][N+n+1];
//
//        //AT
//        a1 = phi[3]*pow(beta[2],0.5*(abs(N-n-B[2])-abs(N-n-1-B[2])+1.0));
//        a2 = phi[0]*pow(beta[2],0.5*(abs(N-n-B[2])-abs(N-n+1-B[2])+1.0));
//        //m[2*N+n+1] [2*N+n+2]  = n*(N-n)*phi[3]*pow(beta[2],0.5*(abs(N-n-B[2])-abs(N-n-1-B[2])+1.0))/(n*phi[3]*pow(beta[2],0.5*(abs(N-n-B[2])-abs(N-n-1-B[2])+1.0))+(N-n)*phi[0]*pow(beta[2],0.5*(abs(N-n-B[2])-abs(N-n+1-B[2])+1.0))); //{(N-n)A,nT} -> {(N-n-1)A,(n+1)T}
//        m[2*N+n+1] [2*N+n+2]  = n*(N-n)*a1 / (n*a1 + (N-n)*a2);
//        //m[2*N+n+1] [2*N+n]    = n*(N-n)*phi[0]*pow(beta[2],0.5*(abs(N-n-B[2])-abs(N-n+1-B[2])+1.0))/(n*phi[3]*pow(beta[2],0.5*(abs(N-n-B[2])-abs(N-n-1-B[2])+1.0))+(N-n)*phi[0]*pow(beta[2],0.5*(abs(N-n-B[2])-abs(N-n+1-B[2])+1.0))); //{(N-n)A,nT} -> {(N-n+1)A,(n-1)T}
//        m[2*N+n+1] [2*N+n]    = n*(N-n)*a2 / (n*a1 + (N-n)*a2);
//        diagonal[2*N+n+1] += m[2*N+n+1][2*N+n+2] + m[2*N+n+1][2*N+n];
//
//        //CG
//        a1 = phi[2]*pow(beta[3],0.5*(abs(N-n-B[3])-abs(N-n-1-B[3])+1.0));
//        a2 = phi[1]*pow(beta[3],0.5*(abs(N-n-B[3])-abs(N-n+1-B[3])+1.0));
//        //m[3*N+n]   [3*N+n+1]  = n*(N-n)*phi[2]*pow(beta[3],0.5*(abs(N-n-B[3])-abs(N-n-1-B[3])+1.0))/(n*phi[2]*pow(beta[3],0.5*(abs(N-n-B[3])-abs(N-n-1-B[3])+1.0))+(N-n)*phi[1]*pow(beta[3],0.5*(abs(N-n-B[3])-abs(N-n+1-B[3])+1.0))); //{(N-n)C,nG} -> {(N-n-1)C,(n+1)G}
//        m[3*N+n]   [3*N+n+1]  = n*(N-n)*a1 / (n*a1 + (N-n)*a2);
//        //m[3*N+n]   [3*N+n-1]  = n*(N-n)*phi[1]*pow(beta[3],0.5*(abs(N-n-B[3])-abs(N-n+1-B[3])+1.0))/(n*phi[2]*pow(beta[3],0.5*(abs(N-n-B[3])-abs(N-n-1-B[3])+1.0))+(N-n)*phi[1]*pow(beta[3],0.5*(abs(N-n-B[3])-abs(N-n+1-B[3])+1.0))); //{(N-n)C,nG} -> {(N-n+1)C,(n-1)G}
//        m[3*N+n]   [3*N+n-1]  = n*(N-n)*a2 / (n*a1 + (N-n)*a2);
//        diagonal[3*N+n] += m[3*N+n][3*N+n+1] + m[3*N+n][3*N+n-1];
//
//        //CT
//        a1 = phi[3]*pow(beta[4],0.5*(abs(N-n-B[4])-abs(N-n-1-B[4])+1.0));
//        a2 = phi[1]*pow(beta[4],0.5*(abs(N-n-B[4])-abs(N-n+1-B[4])+1.0));
//        //m[4*N+n-1] [4*N+n]    = n*(N-n)*phi[3]*pow(beta[4],0.5*(abs(N-n-B[4])-abs(N-n-1-B[4])+1.0))/(n*phi[3]*pow(beta[4],0.5*(abs(N-n-B[4])-abs(N-n-1-B[4])+1.0))+(N-n)*phi[1]*pow(beta[4],0.5*(abs(N-n-B[4])-abs(N-n+1-B[4])+1.0))); //{(N-n)C,nT} -> {(N-n-1)C,(n+1)T}
//        m[4*N+n-1] [4*N+n]    = n*(N-n)*a1 / (n*a1 + (N-n)*a2);
//        //m[4*N+n-1] [4*N+n-2]  = n*(N-n)*phi[1]*pow(beta[4],0.5*(abs(N-n-B[4])-abs(N-n+1-B[4])+1.0))/(n*phi[3]*pow(beta[4],0.5*(abs(N-n-B[4])-abs(N-n-1-B[4])+1.0))+(N-n)*phi[1]*pow(beta[4],0.5*(abs(N-n-B[4])-abs(N-n+1-B[4])+1.0))); //{(N-n)C,nT} -> {(N-n+1)C,(n-1)T}
//        m[4*N+n-1] [4*N+n-2]  = n*(N-n)*a2 / (n*a1 + (N-n)*a2);
//        diagonal[4*N+n-1] += m[4*N+n-1][4*N+n] + m[4*N+n-1][4*N+n-2];
//
//        //GT
//        a1 = phi[3]*pow(beta[5],0.5*(abs(N-n-B[5])-abs(N-n-1-B[5])+1.0));
//        a2 = phi[2]*pow(beta[5],0.5*(abs(N-n-B[5])-abs(N-n+1-B[5])+1.0));
//        //m[5*N+n-2] [5*N+n-1]  = n*(N-n)*phi[3]*pow(beta[5],0.5*(abs(N-n-B[5])-abs(N-n-1-B[5])+1.0))/(n*phi[3]*pow(beta[5],0.5*(abs(N-n-B[5])-abs(N-n-1-B[5])+1.0))+(N-n)*phi[2]*pow(beta[5],0.5*(abs(N-n-B[5])-abs(N-n+1-B[5])+1.0))); //{(N-n)G,nT} -> {(N-n-1)G,(n+1)T}
//        m[5*N+n-2] [5*N+n-1]  = n*(N-n)*a1 / (n*a1 + (N-n)*a2);
//        //m[5*N+n-2] [5*N+n-3]  = n*(N-n)*phi[2]*pow(beta[5],0.5*(abs(N-n-B[5])-abs(N-n+1-B[5])+1.0))/(n*phi[3]*pow(beta[5],0.5*(abs(N-n-B[5])-abs(N-n-1-B[5])+1.0))+(N-n)*phi[2]*pow(beta[5],0.5*(abs(N-n-B[5])-abs(N-n+1-B[5])+1.0))); //{(N-n)G,nT} -> {(N-n+1)G,(n-1)T}
//        m[5*N+n-2] [5*N+n-3]  = n*(N-n)*a2 / (n*a1 + (N-n)*a2);
//        diagonal[5*N+n-2] += m[5*N+n-2][5*N+n-1] + m[5*N+n-2][5*N+n-3];
//
//      }
//
//    }
//
//  }
// Rui's implementation
  m[0][4]      = mu[0];    //{NA} -> {(N-1)A,1C}
  m[1][N+2]    = mu[1];    //{NC} -> {1A,(N-1)C}

  //AG
  m[0][N+3]    = mu[2];    //{NA} -> {(N-1)A,1G}
  m[2][2*N+1]  = mu[3];    //{NG} -> {1A,(N-1)G}

  //AT
  m[0][2*N+2]  = mu[4];    //{NA} -> {(N-1)A,1T}
  m[3][3*N]    = mu[5];    //{NT} -> {1A,(N-1)T}

  //CG
  m[1][3*N+1]  = mu[6];    //{NC} -> {(N-1)C,1aG}
  m[2][4*N-1]  = N*mu[7];    //{NG} -> {1C,(N-1)aG}

  //CT
  m[1][4*N]    = N*mu[8];    //{NC} -> {(N-1)C,1T}
  m[3][5*N-2]  = N*mu[9];    //{NT} -> {1C,(N-1)T}

  //GT
  m[2][5*N-1]  = N*mu[10];    //{NG} -> {(N-1)G,1T}
  m[3][6*N-3]  = N*mu[11];    //{NT} -> {1G,(N-1)T}

  diagonal[0] =  m[0][4]     + m[0][N+3]   + m[0][2*N+2];
  diagonal[1] =  m[1][N+2]   + m[1][3*N+1] + m[1][4*N];
  diagonal[2] =  m[2][2*N+1] + m[2][4*N-1] + m[2][5*N-1];
  diagonal[3] =  m[3][3*N]   + m[3][5*N-2] + m[3][6*N-3];

    //reciprocal of the population size
    double rN = 1.0/(N*N);

    //fixations
    //AC
    m[4]    [0]   = (N-1.0)*(phi[0])*rN;  //{(N-1)A,1C} -> {NA}
    m[N+2]  [1]   = (N-1.0)*(phi[1])*rN;  //{1A,(N-1)C} -> {NC}
    diagonal[4] += m[4][0];
    diagonal[N + 2] += m[N+2]  [1];

    //AG
    m[N+3]  [0]   = (N-1.0)*(phi[0])*rN;  //{(N-1)A,1G} -> {NA}
    m[2*N+1] [2]  = (N-1.0)*(phi[2])*rN;  //{1A,(N-1)G} -> {NG}
    diagonal[N+3] += m[N+3][0];
    diagonal[2*N + 1] += m[2*N+1] [2];
    //AT
    m[2*N+2][0]   = (N-1.0)*(phi[0])*rN;  //{(N-1)A,1T} -> {NA}
    m[3*N]   [3]  = (N-1.0)*(phi[3])*rN;  //{1A,(N-1)T} -> {NT}
    diagonal[2*N+2] += m[2*N+2][0];
    diagonal[3*N] += m[3*N]   [3];
    //CG
    m[3*N+1][1]   = (N-1.0)*(phi[1])*rN;  //{(N-1)C,1G} -> {NC}
    m[4*N-1] [2]  = (N-1.0)*(phi[2])*rN;  //{1C,(N-1)G} -> {NG}
    diagonal[3*N+1] += m[3*N+1][1];
    diagonal[4*N-1] += m[4*N-1] [2];
    //CT
    m[4*N]  [1]   = (N-1.0)*(phi[1])*rN;  //{(N-1)C,1T} -> {NC}
    m[5*N-2] [3]  = (N-1.0)*(phi[3])*rN;  //{1C,(N-1)T} -> {NT}
    diagonal[4*N] += m[3*N+1][1];
    diagonal[5*N-2] += m[5*N-2] [3];
    //GT
    m[5*N-1][2]   = (N-1.0)*(phi[2])*rN;  //{(N-1)G,1T} -> {NG}
    m[6*N-3] [3]  = (N-1.0)*(phi[3])*rN;  //{1G,(N-1)T} -> {NT}
    diagonal[5*N-1] += m[5*N-1][2];
    diagonal[6*N-3] += m[6*N-3] [3];
    //the pomo rate matrix is entirely defined by fixations and mutations if N=2
    if (N>2) {

        //frequency shifts from singletons
        //AC
        m[4]     [5]     = (N-1.0)*(phi[1])*pow(beta[0],0.5*(abs(N-1-B[0])-abs(N-2-B[0])+1.0))*rN;  //{(N-1)A,1C} -> {(N-2)A,2C}
        m[N+2]   [N+1]   = (N-1.0)*(phi[0])*pow(beta[0],0.5*(abs(  1-B[0])-abs(  2-B[0])+1.0))*rN;  //{1A,(N-1)C} -> {2A,(N-2)C}
        diagonal[4] += m[4][5];
        diagonal[N+2] += m[N+2]   [N+1];
        //AG
        m[N+3]   [N+4]   = (N-1.0)*(phi[2])*pow(beta[1],0.5*(abs(N-1-B[1])-abs(N-2-B[1])+1.0))*rN;  //{(N-1)A,1G} -> {(N-2)A,2G}
        m[2*N+1] [2*N]   = (N-1.0)*(phi[0])*pow(beta[1],0.5*(abs(  1-B[1])-abs(  2-B[1])+1.0))*rN;  //{1A,(N-1)G} -> {2A,(N-2)G}
        diagonal[N+3] += m[N+3]   [N+4];
        diagonal[2*N+1] += m[2*N+1] [2*N];
        //AT
        m[2*N+2] [2*N+3] = (N-1.0)*(phi[3])*pow(beta[2],0.5*(abs(N-1-B[2])-abs(N-2-B[2])+1.0))*rN;  //{(N-1)A,1T} -> {(N-2)A,2T}
        m[3*N]   [3*N-1] = (N-1.0)*(phi[0])*pow(beta[2],0.5*(abs(  1-B[2])-abs(  2-B[2])+1.0))*rN;  //{1A,(N-1)T} -> {2A,(N-2)T}
        diagonal[2*N+2] += m[2*N+2] [2*N+3];
        diagonal[3*N] += m[3*N]   [3*N-1];
        //CG
        m[3*N+1] [3*N+2] = (N-1.0)*(phi[2])*pow(beta[3],0.5*(abs(N-1-B[3])-abs(N-2-B[3])+1.0))*rN;  //{(N-1)C,1G} -> {(N-2)C,2G}
        m[4*N-1] [4*N-2] = (N-1.0)*(phi[1])*pow(beta[3],0.5*(abs(  1-B[3])-abs(  2-B[3])+1.0))*rN;  //{1C,(N-1)G} -> {2C,(N-2)G}
        diagonal[3*N+1] += m[3*N+1] [3*N+2];
        diagonal[4*N-1] += m[4*N-1] [4*N-2];
        //CT
        m[4*N]   [4*N+1] = (N-1.0)*(phi[3])*pow(beta[4],0.5*(abs(N-1-B[4])-abs(N-2-B[4])+1.0))*rN;  //{(N-1)C,1T} -> {(N-2)C,2T}
        m[5*N-2] [5*N-3] = (N-1.0)*(phi[1])*pow(beta[4],0.5*(abs(  1-B[4])-abs(  2-B[4])+1.0))*rN;  //{1C,(N-1)T} -> {2C,(N-2)T}
        diagonal[4*N] += m[4*N]   [4*N+1];
        diagonal[5*N-2] += m[5*N-2] [5*N-3];
        //GT
        m[5*N-1] [5*N]   = (N-1.0)*(phi[3])*pow(beta[5],0.5*(abs(N-1-B[5])-abs(N-2-B[5])+1.0))*rN;  //{(N-1)G,1T} -> {(N-2)G,2T}
        m[6*N-3] [6*N-4] = (N-1.0)*(phi[2])*pow(beta[5],0.5*(abs(  1-B[5])-abs(  2-B[5])+1.0))*rN;  //{1G,(N-1)T} -> {2G,(N-2)T}
        diagonal[5*N-1] += m[5*N-1] [5*N];
        diagonal[6*N-3] += m[6*N-3] [6*N-4];

        //frequency shifts for all the other polymorphic states
        if (N>3) {

            //polymorphic states are populated in two fronts, thus the need for the middle frequency
            int S = N/2+1;

            for (int n=2; n<S; n++){

                //populates the first half of the polymorphic edges
                //AC
                m[n+3]     [n+4]      = n*(N-n)*(phi[1])*pow(beta[0],0.5*(abs(N-n-B[0])-abs(N-n-1-B[0])+1.0))*rN; //{(N-n)A,nC} -> {(N-n-1)A,(n-1)C}
                m[n+3]     [n+2]      = n*(N-n)*(phi[0])*pow(beta[0],0.5*(abs(N-n-B[0])-abs(N-n+1-B[0])+1.0))*rN; //{(N-n)A,nC} -> {(N-n+1)A,(n+1)C}
                diagonal[n+3] += m[n+3]     [n+4] + m[n+3]     [n+2];
                //AG
                m[N+n+2]   [N+n+3]    = n*(N-n)*(phi[2])*pow(beta[1],0.5*(abs(N-n-B[1])-abs(N-n-1-B[1])+1.0))*rN; //{(N-n)A,nG} -> {(N-n-1)A,(n-1)G}
                m[N+n+2]   [N+n+1]    = n*(N-n)*(phi[0])*pow(beta[1],0.5*(abs(N-n-B[1])-abs(N-n+1-B[1])+1.0))*rN; //{(N-n)A,nG} -> {(N-n+1)A,(n+1)G}
                diagonal[N+n+2] += m[N+n+2]   [N+n+3] + m[N+n+2]   [N+n+1];

                //AT
                m[2*N+n+1] [2*N+n+2]  = n*(N-n)*(phi[3])*pow(beta[2],0.5*(abs(N-n-B[2])-abs(N-n-1-B[2])+1.0))*rN; //{(N-n)A,nT} -> {(N-n-1)A,(n-1)T}
                m[2*N+n+1] [2*N+n]    = n*(N-n)*(phi[0])*pow(beta[2],0.5*(abs(N-n-B[2])-abs(N-n+1-B[2])+1.0))*rN; //{(N-n)A,nT} -> {(N-n+1)A,(n+1)T}
                diagonal[2*N+n+1] += m[2*N+n+1] [2*N+n+2] + m[2*N+n+1] [2*N+n];

                //CG
                m[3*N+n]   [3*N+n+1]  = n*(N-n)*(phi[2])*pow(beta[3],0.5*(abs(N-n-B[3])-abs(N-n-1-B[3])+1.0))*rN; //{(N-n)C,nG} -> {(N-n-1)C,(n-1)G}
                m[3*N+n]   [3*N+n-1]  = n*(N-n)*(phi[1])*pow(beta[3],0.5*(abs(N-n-B[3])-abs(N-n+1-B[3])+1.0))*rN; //{(N-n)C,nG} -> {(N-n+1)C,(n+1)G}
                diagonal[3*N+n] += m[3*N+n]   [3*N+n+1] + m[3*N+n]   [3*N+n-1];

                //CT
                m[4*N+n-1] [4*N+n]    = n*(N-n)*(phi[3])*pow(beta[4],0.5*(abs(N-n-B[4])-abs(N-n-1-B[4])+1.0))*rN; //{(N-n)C,nT} -> {(N-n-1)C,(n-1)T}
                m[4*N+n-1] [4*N+n-2]  = n*(N-n)*(phi[1])*pow(beta[4],0.5*(abs(N-n-B[4])-abs(N-n+1-B[4])+1.0))*rN; //{(N-n)C,nT} -> {(N-n+1)C,(n+1)T}
                diagonal[4*N+n-1] += m[4*N+n-1] [4*N+n] + m[4*N+n-1] [4*N+n-2];

                //GT
                m[5*N+n-2] [5*N+n-1]  = n*(N-n)*(phi[3])*pow(beta[5],0.5*(abs(N-n-B[5])-abs(N-n-1-B[5])+1.0))*rN; //{(N-n)G,nT} -> {(N-n-1)G,(n-1)T}
                m[5*N+n-2] [5*N+n-3]  = n*(N-n)*(phi[2])*pow(beta[5],0.5*(abs(N-n-B[5])-abs(N-n+1-B[5])+1.0))*rN; //{(N-n)G,nT} -> {(N-n+1)G,(n+1)T}
                diagonal[5*N+n-2] += m[5*N+n-2] [5*N+n-1] + m[5*N+n-2] [5*N+n-3];


                //populates the second half of the polymorphic edges
                //AC
                m[N-n+3] [N-n+2]     = (N-n)*n*(phi[0])*pow(beta[0],0.5*(abs(n-B[0])-abs(n+1-B[0])+1.0))*rN; //{nA,(N-n)C} -> {(n+1)A,(N-n+1)C}
                m[N-n+3] [N-n+4]     = (N-n)*n*(phi[1])*pow(beta[0],0.5*(abs(n-B[0])-abs(n-1-B[0])+1.0))*rN; //{nA,(N-n)C} -> {(n-1)A,(N-n-1)C}
                diagonal[N-n+3] += m[N-n+3] [N-n+2] + m[N-n+3] [N-n+4];

                //AG
                m[2*N-n+2] [2*N-n+1] = (N-n)*n*(phi[0])*pow(beta[1],0.5*(abs(n-B[1])-abs(n+1-B[1])+1.0))*rN; //{nA,(N-n)G} -> {(n+1)A,(N-n+1)G}
                m[2*N-n+2] [2*N-n+3] = (N-n)*n*(phi[2])*pow(beta[1],0.5*(abs(n-B[1])-abs(n-1-B[1])+1.0))*rN; //{nA,(N-n)G} -> {(n-1)A,(N-n-1)G}
                diagonal[2*N-n+2] += m[2*N-n+2] [2*N-n+1] + m[2*N-n+2] [2*N-n+3];

                //AT
                m[3*N-n+1] [3*N-n]   = (N-n)*n*(phi[0])*pow(beta[2],0.5*(abs(n-B[2])-abs(n+1-B[2])+1.0))*rN; //{nA,(N-n)T} -> {(n+1)A,(N-n+1)T}
                m[3*N-n+1] [3*N-n+2] = (N-n)*n*(phi[3])*pow(beta[2],0.5*(abs(n-B[2])-abs(n-1-B[2])+1.0))*rN; //{nA,(N-n)T} -> {(n-1)A,(N-n-1)T}
                diagonal[3*N-n+1] += m[3*N-n+1] [3*N-n] + m[3*N-n+1] [3*N-n+2];

                //CG
                m[4*N-n] [4*N-n-1]   = (N-n)*n*(phi[1])*pow(beta[3],0.5*(abs(n-B[3])-abs(n+1-B[3])+1.0))*rN; //{nC,(N-n)G} -> {(n+1)C,(N-n+1)G}
                m[4*N-n] [4*N-n+1]   = (N-n)*n*(phi[2])*pow(beta[3],0.5*(abs(n-B[3])-abs(n-1-B[3])+1.0))*rN; //{nC,(N-n)G} -> {(n-1)C,(N-n-1)G}
                diagonal[4*N-n] += m[4*N-n] [4*N-n-1] + m[4*N-n] [4*N-n+1];

                //CT
                m[5*N-n-1] [5*N-n-2] = (N-n)*n*(phi[1])*pow(beta[4],0.5*(abs(n-B[4])-abs(n+1-B[4])+1.0))*rN; //{nC,(N-n)T} -> {(n+1)C,(N-n+1)T}
                m[5*N-n-1] [5*N-n]   = (N-n)*n*(phi[3])*pow(beta[4],0.5*(abs(n-B[4])-abs(n-1-B[4])+1.0))*rN; //{nC,(N-n)T} -> {(n-1)C,(N-n-1)T}
                diagonal[5*N-n-1] += m[5*N-n-1] [5*N-n-2] + m[5*N-n-1] [5*N-n];

                //GT
                m[6*N-n-2] [6*N-n-3] = (N-n)*n*(phi[2])*pow(beta[5],0.5*(abs(n-B[5])-abs(n+1-B[5])+1.0))*rN; //{nG,(N-n)T} -> {(n+1)G,(N-n+1)T}
                m[6*N-n-2] [6*N-n-1] = (N-n)*n*(phi[3])*pow(beta[5],0.5*(abs(n-B[5])-abs(n-1-B[5])+1.0))*rN; //{nG,(N-n)T} -> {(n-1)G,(N-n-1)T}
                diagonal[6*N-n-2] += m[6*N-n-2] [6*N-n-3] + m[6*N-n-2] [6*N-n-1];

            }

        }

    }

  // set the diagonal values and calculate the reciprocal of the average rate
  for (int j=0; j< num_states; j++){
    m[j][j] = -diagonal[j];
  }
  
    
  /*
  //I cannot get the stationary frequencies by any means. So, I am just skiping the matrix normalization.
  std::vector< double > stationaryFrequencies = getStationaryFrequencies();
  double rRate = 0.0;
  for (int j=0; j< num_states; j++){
    rRate += diagonal[j]*stationaryVector[j];   
    std::cout << "rate:" << stationaryVector[j] << "\n";
  }

  rRate = 1.0/rRate;
  */


}


/** Calculate the transition probabilities */
void RateMatrix_PoMoBalance4N::calculateTransitionProbabilities(double startAge, double endAge, double rate, TransitionProbabilityMatrix& P) const
{

  // We use repeated squaring to quickly obtain exponentials, as in Poujol and Lartillot, Bioinformatics 2014.
  // Mayrose et al. 2010 also used this method for chromosome evolution (named the squaring and scaling method in Moler and Van Loan 2003).
  double t = rate * (startAge - endAge);
  exponentiateMatrixByScalingAndSquaring(t, P );

  return;
}


RateMatrix_PoMoBalance4N* RateMatrix_PoMoBalance4N::clone( void ) const
{
    return new RateMatrix_PoMoBalance4N( *this );
}


// We comment out this function because the stationary frequencies in this case seem intractable, will need to review
// this in the future
//std::vector<double> RateMatrix_PoMoBalance4N::getStationaryFrequencies( void ) const
//{
//  return stationaryVector;
//}

std::vector<double> RateMatrix_PoMoBalance4N::getStationaryFrequencies( void ) const
{
    return calculateStationaryFrequencies();
}




void RateMatrix_PoMoBalance4N::setN( long & ni )
{
    N = ni;
    
    // set flags
    needs_update = true;
    
}


void RateMatrix_PoMoBalance4N::setMu( const std::vector<double> &m )
{
    mu = m;
    
    // set flags
    needs_update = true;
}

void RateMatrix_PoMoBalance4N::setPhi( const std::vector<double> &f )
{
    phi = f;
    
    // set flags
    needs_update = true;
}

void RateMatrix_PoMoBalance4N::setBeta( const std::vector<double> &b )
{
    beta = b;

    // set flags
    needs_update = true;
}

void RateMatrix_PoMoBalance4N::setB( const std::vector<long> &Bf )
{
    B = Bf;

    // set flags
    needs_update = true;
}


void RateMatrix_PoMoBalance4N::update( void )
{

    if ( needs_update )
    {

        buildRateMatrix();

        // rescale: not useful, same loglk.
        //rescaleToAverageRate( 1.0 );

        // clean flags
        needs_update = false;
    }
}
