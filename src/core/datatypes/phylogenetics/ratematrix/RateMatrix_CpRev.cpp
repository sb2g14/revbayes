#include "RateMatrix_CpRev.h"
#include "RbVectorUtilities.h"


using namespace RevBayesCore;

/** Construct rate matrix with n states */
RateMatrix_CpRev::RateMatrix_CpRev( void ) : RateMatrix_Empirical( 20 ){
    
    MatrixReal &m = *the_rate_matrix;
    
    /* CpRev */	    
	m[ 0][ 0] =    0; m[ 0][ 1] =  105; m[ 0][ 2] =  227; m[ 0][ 3] =  175; m[ 0][ 4] =  669; 
	m[ 0][ 5] =  157; m[ 0][ 6] =  499; m[ 0][ 7] =  665; m[ 0][ 8] =   66; m[ 0][ 9] =  145; 
	m[ 0][10] =  197; m[ 0][11] =  236; m[ 0][12] =  185; m[ 0][13] =   68; m[ 0][14] =  490; 
	m[ 0][15] = 2440; m[ 0][16] = 1340; m[ 0][17] =   14; m[ 0][18] =   56; m[ 0][19] =  968; 
	m[ 1][ 0] =  105; m[ 1][ 1] =    0; m[ 1][ 2] =  357; m[ 1][ 3] =   43; m[ 1][ 4] =  823; 
	m[ 1][ 5] = 1745; m[ 1][ 6] =  152; m[ 1][ 7] =  243; m[ 1][ 8] =  715; m[ 1][ 9] =  136; 
	m[ 1][10] =  203; m[ 1][11] = 4482; m[ 1][12] =  125; m[ 1][13] =   53; m[ 1][14] =   87; 
	m[ 1][15] =  385; m[ 1][16] =  314; m[ 1][17] =  230; m[ 1][18] =  323; m[ 1][19] =   92; 
	m[ 2][ 0] =  227; m[ 2][ 1] =  357; m[ 2][ 2] =    0; m[ 2][ 3] = 4435; m[ 2][ 4] =  538; 
	m[ 2][ 5] =  768; m[ 2][ 6] = 1055; m[ 2][ 7] =  653; m[ 2][ 8] = 1405; m[ 2][ 9] =  168; 
	m[ 2][10] =  113; m[ 2][11] = 2430; m[ 2][12] =   61; m[ 2][13] =   97; m[ 2][14] =  173; 
	m[ 2][15] = 2085; m[ 2][16] = 1393; m[ 2][17] =   40; m[ 2][18] =  754; m[ 2][19] =   83; 
	m[ 3][ 0] =  175; m[ 3][ 1] =   43; m[ 3][ 2] = 4435; m[ 3][ 3] =    0; m[ 3][ 4] =   10; 
	m[ 3][ 5] =  400; m[ 3][ 6] = 3691; m[ 3][ 7] =  431; m[ 3][ 8] =  331; m[ 3][ 9] =   10; 
	m[ 3][10] =   10; m[ 3][11] =  412; m[ 3][12] =   47; m[ 3][13] =   22; m[ 3][14] =  170; 
	m[ 3][15] =  590; m[ 3][16] =  266; m[ 3][17] =   18; m[ 3][18] =  281; m[ 3][19] =   75; 
	m[ 4][ 0] =  669; m[ 4][ 1] =  823; m[ 4][ 2] =  538; m[ 4][ 3] =   10; m[ 4][ 4] =    0; 
	m[ 4][ 5] =   10; m[ 4][ 6] =   10; m[ 4][ 7] =  303; m[ 4][ 8] =  441; m[ 4][ 9] =  280; 
	m[ 4][10] =  396; m[ 4][11] =   48; m[ 4][12] =  159; m[ 4][13] =  726; m[ 4][14] =  285; 
	m[ 4][15] = 2331; m[ 4][16] =  576; m[ 4][17] =  435; m[ 4][18] = 1466; m[ 4][19] =  592; 
	m[ 5][ 0] =  157; m[ 5][ 1] = 1745; m[ 5][ 2] =  768; m[ 5][ 3] =  400; m[ 5][ 4] =   10; 
	m[ 5][ 5] =    0; m[ 5][ 6] = 3122; m[ 5][ 7] =  133; m[ 5][ 8] = 1269; m[ 5][ 9] =   92; 
	m[ 5][10] =  286; m[ 5][11] = 3313; m[ 5][12] =  202; m[ 5][13] =   10; m[ 5][14] =  323; 
	m[ 5][15] =  396; m[ 5][16] =  241; m[ 5][17] =   53; m[ 5][18] =  391; m[ 5][19] =   54; 
	m[ 6][ 0] =  499; m[ 6][ 1] =  152; m[ 6][ 2] = 1055; m[ 6][ 3] = 3691; m[ 6][ 4] =   10; 
	m[ 6][ 5] = 3122; m[ 6][ 6] =    0; m[ 6][ 7] =  379; m[ 6][ 8] =  162; m[ 6][ 9] =  148; 
	m[ 6][10] =   82; m[ 6][11] = 2629; m[ 6][12] =  113; m[ 6][13] =  145; m[ 6][14] =  185; 
	m[ 6][15] =  568; m[ 6][16] =  369; m[ 6][17] =   63; m[ 6][18] =  142; m[ 6][19] =  200; 
	m[ 7][ 0] =  665; m[ 7][ 1] =  243; m[ 7][ 2] =  653; m[ 7][ 3] =  431; m[ 7][ 4] =  303; 
	m[ 7][ 5] =  133; m[ 7][ 6] =  379; m[ 7][ 7] =    0; m[ 7][ 8] =   19; m[ 7][ 9] =   40; 
	m[ 7][10] =   20; m[ 7][11] =  263; m[ 7][12] =   21; m[ 7][13] =   25; m[ 7][14] =   28; 
	m[ 7][15] =  691; m[ 7][16] =   92; m[ 7][17] =   82; m[ 7][18] =   10; m[ 7][19] =   91; 
	m[ 8][ 0] =   66; m[ 8][ 1] =  715; m[ 8][ 2] = 1405; m[ 8][ 3] =  331; m[ 8][ 4] =  441; 
	m[ 8][ 5] = 1269; m[ 8][ 6] =  162; m[ 8][ 7] =   19; m[ 8][ 8] =    0; m[ 8][ 9] =   29; 
	m[ 8][10] =   66; m[ 8][11] =  305; m[ 8][12] =   10; m[ 8][13] =  127; m[ 8][14] =  152; 
	m[ 8][15] =  303; m[ 8][16] =   32; m[ 8][17] =   69; m[ 8][18] = 1971; m[ 8][19] =   25; 
	m[ 9][ 0] =  145; m[ 9][ 1] =  136; m[ 9][ 2] =  168; m[ 9][ 3] =   10; m[ 9][ 4] =  280; 
	m[ 9][ 5] =   92; m[ 9][ 6] =  148; m[ 9][ 7] =   40; m[ 9][ 8] =   29; m[ 9][ 9] =    0; 
	m[ 9][10] = 1745; m[ 9][11] =  345; m[ 9][12] = 1772; m[ 9][13] =  454; m[ 9][14] =  117; 
	m[ 9][15] =  216; m[ 9][16] = 1040; m[ 9][17] =   42; m[ 9][18] =   89; m[ 9][19] = 4797; 
	m[10][ 0] =  197; m[10][ 1] =  203; m[10][ 2] =  113; m[10][ 3] =   10; m[10][ 4] =  396; 
	m[10][ 5] =  286; m[10][ 6] =   82; m[10][ 7] =   20; m[10][ 8] =   66; m[10][ 9] = 1745; 
	m[10][10] =    0; m[10][11] =  218; m[10][12] = 1351; m[10][13] = 1268; m[10][14] =  219; 
	m[10][15] =  516; m[10][16] =  156; m[10][17] =  159; m[10][18] =  189; m[10][19] =  865; 
	m[11][ 0] =  236; m[11][ 1] = 4482; m[11][ 2] = 2430; m[11][ 3] =  412; m[11][ 4] =   48; 
	m[11][ 5] = 3313; m[11][ 6] = 2629; m[11][ 7] =  263; m[11][ 8] =  305; m[11][ 9] =  345; 
	m[11][10] =  218; m[11][11] =    0; m[11][12] =  193; m[11][13] =   72; m[11][14] =  302; 
	m[11][15] =  868; m[11][16] =  918; m[11][17] =   10; m[11][18] =  247; m[11][19] =  249; 
	m[12][ 0] =  185; m[12][ 1] =  125; m[12][ 2] =   61; m[12][ 3] =   47; m[12][ 4] =  159; 
	m[12][ 5] =  202; m[12][ 6] =  113; m[12][ 7] =   21; m[12][ 8] =   10; m[12][ 9] = 1772; 
	m[12][10] = 1351; m[12][11] =  193; m[12][12] =    0; m[12][13] =  327; m[12][14] =  100; 
	m[12][15] =   93; m[12][16] =  645; m[12][17] =   86; m[12][18] =  215; m[12][19] =  475; 
	m[13][ 0] =   68; m[13][ 1] =   53; m[13][ 2] =   97; m[13][ 3] =   22; m[13][ 4] =  726; 
	m[13][ 5] =   10; m[13][ 6] =  145; m[13][ 7] =   25; m[13][ 8] =  127; m[13][ 9] =  454; 
	m[13][10] = 1268; m[13][11] =   72; m[13][12] =  327; m[13][13] =    0; m[13][14] =   43; 
	m[13][15] =  487; m[13][16] =  148; m[13][17] =  468; m[13][18] = 2370; m[13][19] =  317; 
	m[14][ 0] =  490; m[14][ 1] =   87; m[14][ 2] =  173; m[14][ 3] =  170; m[14][ 4] =  285; 
	m[14][ 5] =  323; m[14][ 6] =  185; m[14][ 7] =   28; m[14][ 8] =  152; m[14][ 9] =  117; 
	m[14][10] =  219; m[14][11] =  302; m[14][12] =  100; m[14][13] =   43; m[14][14] =    0; 
	m[14][15] = 1202; m[14][16] =  260; m[14][17] =   49; m[14][18] =   97; m[14][19] =  122; 
	m[15][ 0] = 2440; m[15][ 1] =  385; m[15][ 2] = 2085; m[15][ 3] =  590; m[15][ 4] = 2331; 
	m[15][ 5] =  396; m[15][ 6] =  568; m[15][ 7] =  691; m[15][ 8] =  303; m[15][ 9] =  216; 
	m[15][10] =  516; m[15][11] =  868; m[15][12] =   93; m[15][13] =  487; m[15][14] = 1202; 
	m[15][15] =    0; m[15][16] = 2151; m[15][17] =   73; m[15][18] =  522; m[15][19] =  167; 
	m[16][ 0] = 1340; m[16][ 1] =  314; m[16][ 2] = 1393; m[16][ 3] =  266; m[16][ 4] =  576; 
	m[16][ 5] =  241; m[16][ 6] =  369; m[16][ 7] =   92; m[16][ 8] =   32; m[16][ 9] = 1040; 
	m[16][10] =  156; m[16][11] =  918; m[16][12] =  645; m[16][13] =  148; m[16][14] =  260; 
	m[16][15] = 2151; m[16][16] =    0; m[16][17] =   29; m[16][18] =   71; m[16][19] =  760; 
	m[17][ 0] =   14; m[17][ 1] =  230; m[17][ 2] =   40; m[17][ 3] =   18; m[17][ 4] =  435; 
	m[17][ 5] =   53; m[17][ 6] =   63; m[17][ 7] =   82; m[17][ 8] =   69; m[17][ 9] =   42; 
	m[17][10] =  159; m[17][11] =   10; m[17][12] =   86; m[17][13] =  468; m[17][14] =   49; 
	m[17][15] =   73; m[17][16] =   29; m[17][17] =    0; m[17][18] =  346; m[17][19] =   10; 
	m[18][ 0] =   56; m[18][ 1] =  323; m[18][ 2] =  754; m[18][ 3] =  281; m[18][ 4] = 1466; 
	m[18][ 5] =  391; m[18][ 6] =  142; m[18][ 7] =   10; m[18][ 8] = 1971; m[18][ 9] =   89; 
	m[18][10] =  189; m[18][11] =  247; m[18][12] =  215; m[18][13] = 2370; m[18][14] =   97; 
	m[18][15] =  522; m[18][16] =   71; m[18][17] =  346; m[18][18] =    0; m[18][19] =  119; 
	m[19][ 0] =  968; m[19][ 1] =   92; m[19][ 2] =   83; m[19][ 3] =   75; m[19][ 4] =  592; 
	m[19][ 5] =   54; m[19][ 6] =  200; m[19][ 7] =   91; m[19][ 8] =   25; m[19][ 9] = 4797; 
	m[19][10] =  865; m[19][11] =  249; m[19][12] =  475; m[19][13] =  317; m[19][14] =  122; 
	m[19][15] =  167; m[19][16] =  760; m[19][17] =   10; m[19][18] =  119; m[19][19] =    0; 
    
	stationary_freqs[0] = 0.076;
	stationary_freqs[1] = 0.062;
	stationary_freqs[2] = 0.041;
	stationary_freqs[3] = 0.037;
	stationary_freqs[4] = 0.009;
	stationary_freqs[5] = 0.038;
	stationary_freqs[6] = 0.049;
	stationary_freqs[7] = 0.084;
	stationary_freqs[8] = 0.025;
	stationary_freqs[9] = 0.081;
	stationary_freqs[10] = 0.101;
	stationary_freqs[11] = 0.050;
	stationary_freqs[12] = 0.022;
	stationary_freqs[13] = 0.051;
	stationary_freqs[14] = 0.043;
	stationary_freqs[15] = 0.062;
	stationary_freqs[16] = 0.054;
	stationary_freqs[17] = 0.018;
	stationary_freqs[18] = 0.031;
	stationary_freqs[19] = 0.066;
   
    VectorUtilities::normalize(stationary_freqs);

    // multiply stationary frequencies into exchangeability matrix
    for (size_t i = 0; i < 20; i++)
    {
        for (size_t j = 0; j < 20; j++)
        {
            m[i][j] *= stationary_freqs[j];
        }
    }
    
    // set the diagonal values
    setDiagonal();
    
    // rescale 
    rescaleToAverageRate( 1.0 );
    
    // update the eigensystem
    updateEigenSystem();
    
}


/** Destructor */
RateMatrix_CpRev::~RateMatrix_CpRev(void) {
    
}




RateMatrix_CpRev* RateMatrix_CpRev::clone( void ) const {
    return new RateMatrix_CpRev( *this );
}


