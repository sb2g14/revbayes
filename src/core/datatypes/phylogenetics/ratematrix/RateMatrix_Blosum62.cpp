#include "RateMatrix_Blosum62.h"
#include "RbVectorUtilities.h"


using namespace RevBayesCore;

/** Construct rate matrix with n states */
RateMatrix_Blosum62::RateMatrix_Blosum62( void ) : RateMatrix_Empirical( 20 )
{
    
    MatrixReal &m = *the_rate_matrix;
    
    /* Blosum62 */	    
    m[ 0][ 0] = 0.000000000000; m[ 0][ 1] = 0.735790389698; m[ 0][ 2] = 0.485391055466; m[ 0][ 3] = 0.543161820899; m[ 0][ 4] = 1.459995310470; 
	m[ 0][ 5] = 1.199705704602; m[ 0][ 6] = 1.170949042800; m[ 0][ 7] = 1.955883574960; m[ 0][ 8] = 0.716241444998; m[ 0][ 9] = 0.605899003687; 
	m[ 0][10] = 0.800016530518; m[ 0][11] = 1.295201266783; m[ 0][12] = 1.253758266664; m[ 0][13] = 0.492964679748; m[ 0][14] = 1.173275900924; 
	m[ 0][15] = 4.325092687057; m[ 0][16] = 1.729178019485; m[ 0][17] = 0.465839367725; m[ 0][18] = 0.718206697586; m[ 0][19] = 2.187774522005; 
	m[ 1][ 0] = 0.735790389698; m[ 1][ 1] = 0.000000000000; m[ 1][ 2] = 1.297446705134; m[ 1][ 3] = 0.500964408555; m[ 1][ 4] = 0.227826574209; 
	m[ 1][ 5] = 3.020833610064; m[ 1][ 6] = 1.360574190420; m[ 1][ 7] = 0.418763308518; m[ 1][ 8] = 1.456141166336; m[ 1][ 9] = 0.232036445142; 
	m[ 1][10] = 0.622711669692; m[ 1][11] = 5.411115141489; m[ 1][12] = 0.983692987457; m[ 1][13] = 0.371644693209; m[ 1][14] = 0.448133661718; 
	m[ 1][15] = 1.122783104210; m[ 1][16] = 0.914665954563; m[ 1][17] = 0.426382310122; m[ 1][18] = 0.720517441216; m[ 1][19] = 0.438388343772; 
	m[ 2][ 0] = 0.485391055466; m[ 2][ 1] = 1.297446705134; m[ 2][ 2] = 0.000000000000; m[ 2][ 3] = 3.180100048216; m[ 2][ 4] = 0.397358949897; 
	m[ 2][ 5] = 1.839216146992; m[ 2][ 6] = 1.240488508640; m[ 2][ 7] = 1.355872344485; m[ 2][ 8] = 2.414501434208; m[ 2][ 9] = 0.283017326278; 
	m[ 2][10] = 0.211888159615; m[ 2][11] = 1.593137043457; m[ 2][12] = 0.648441278787; m[ 2][13] = 0.354861249223; m[ 2][14] = 0.494887043702; 
	m[ 2][15] = 2.904101656456; m[ 2][16] = 1.898173634533; m[ 2][17] = 0.191482046247; m[ 2][18] = 0.538222519037; m[ 2][19] = 0.312858797993; 
	m[ 3][ 0] = 0.543161820899; m[ 3][ 1] = 0.500964408555; m[ 3][ 2] = 3.180100048216; m[ 3][ 3] = 0.000000000000; m[ 3][ 4] = 0.240836614802; 
	m[ 3][ 5] = 1.190945703396; m[ 3][ 6] = 3.761625208368; m[ 3][ 7] = 0.798473248968; m[ 3][ 8] = 0.778142664022; m[ 3][ 9] = 0.418555732462; 
	m[ 3][10] = 0.218131577594; m[ 3][11] = 1.032447924952; m[ 3][12] = 0.222621897958; m[ 3][13] = 0.281730694207; m[ 3][14] = 0.730628272998; 
	m[ 3][15] = 1.582754142065; m[ 3][16] = 0.934187509431; m[ 3][17] = 0.145345046279; m[ 3][18] = 0.261422208965; m[ 3][19] = 0.258129289418; 
	m[ 4][ 0] = 1.459995310470; m[ 4][ 1] = 0.227826574209; m[ 4][ 2] = 0.397358949897; m[ 4][ 3] = 0.240836614802; m[ 4][ 4] = 0.000000000000; 
	m[ 4][ 5] = 0.329801504630; m[ 4][ 6] = 0.140748891814; m[ 4][ 7] = 0.418203192284; m[ 4][ 8] = 0.354058109831; m[ 4][ 9] = 0.774894022794; 
	m[ 4][10] = 0.831842640142; m[ 4][11] = 0.285078800906; m[ 4][12] = 0.767688823480; m[ 4][13] = 0.441337471187; m[ 4][14] = 0.356008498769; 
	m[ 4][15] = 1.197188415094; m[ 4][16] = 1.119831358516; m[ 4][17] = 0.527664418872; m[ 4][18] = 0.470237733696; m[ 4][19] = 1.116352478606; 
	m[ 5][ 0] = 1.199705704602; m[ 5][ 1] = 3.020833610064; m[ 5][ 2] = 1.839216146992; m[ 5][ 3] = 1.190945703396; m[ 5][ 4] = 0.329801504630; 
	m[ 5][ 5] = 0.000000000000; m[ 5][ 6] = 5.528919177928; m[ 5][ 7] = 0.609846305383; m[ 5][ 8] = 2.435341131140; m[ 5][ 9] = 0.236202451204; 
	m[ 5][10] = 0.580737093181; m[ 5][11] = 3.945277674515; m[ 5][12] = 2.494896077113; m[ 5][13] = 0.144356959750; m[ 5][14] = 0.858570575674; 
	m[ 5][15] = 1.934870924596; m[ 5][16] = 1.277480294596; m[ 5][17] = 0.758653808642; m[ 5][18] = 0.958989742850; m[ 5][19] = 0.530785790125; 
	m[ 6][ 0] = 1.170949042800; m[ 6][ 1] = 1.360574190420; m[ 6][ 2] = 1.240488508640; m[ 6][ 3] = 3.761625208368; m[ 6][ 4] = 0.140748891814; 
	m[ 6][ 5] = 5.528919177928; m[ 6][ 6] = 0.000000000000; m[ 6][ 7] = 0.423579992176; m[ 6][ 8] = 1.626891056982; m[ 6][ 9] = 0.186848046932; 
	m[ 6][10] = 0.372625175087; m[ 6][11] = 2.802427151679; m[ 6][12] = 0.555415397470; m[ 6][13] = 0.291409084165; m[ 6][14] = 0.926563934846; 
	m[ 6][15] = 1.769893238937; m[ 6][16] = 1.071097236007; m[ 6][17] = 0.407635648938; m[ 6][18] = 0.596719300346; m[ 6][19] = 0.524253846338; 
	m[ 7][ 0] = 1.955883574960; m[ 7][ 1] = 0.418763308518; m[ 7][ 2] = 1.355872344485; m[ 7][ 3] = 0.798473248968; m[ 7][ 4] = 0.418203192284; 
	m[ 7][ 5] = 0.609846305383; m[ 7][ 6] = 0.423579992176; m[ 7][ 7] = 0.000000000000; m[ 7][ 8] = 0.539859124954; m[ 7][ 9] = 0.189296292376; 
	m[ 7][10] = 0.217721159236; m[ 7][11] = 0.752042440303; m[ 7][12] = 0.459436173579; m[ 7][13] = 0.368166464453; m[ 7][14] = 0.504086599527; 
	m[ 7][15] = 1.509326253224; m[ 7][16] = 0.641436011405; m[ 7][17] = 0.508358924638; m[ 7][18] = 0.308055737035; m[ 7][19] = 0.253340790190; 
	m[ 8][ 0] = 0.716241444998; m[ 8][ 1] = 1.456141166336; m[ 8][ 2] = 2.414501434208; m[ 8][ 3] = 0.778142664022; m[ 8][ 4] = 0.354058109831; 
	m[ 8][ 5] = 2.435341131140; m[ 8][ 6] = 1.626891056982; m[ 8][ 7] = 0.539859124954; m[ 8][ 8] = 0.000000000000; m[ 8][ 9] = 0.252718447885; 
	m[ 8][10] = 0.348072209797; m[ 8][11] = 1.022507035889; m[ 8][12] = 0.984311525359; m[ 8][13] = 0.714533703928; m[ 8][14] = 0.527007339151; 
	m[ 8][15] = 1.117029762910; m[ 8][16] = 0.585407090225; m[ 8][17] = 0.301248600780; m[ 8][18] = 4.218953969389; m[ 8][19] = 0.201555971750; 
	m[ 9][ 0] = 0.605899003687; m[ 9][ 1] = 0.232036445142; m[ 9][ 2] = 0.283017326278; m[ 9][ 3] = 0.418555732462; m[ 9][ 4] = 0.774894022794; 
	m[ 9][ 5] = 0.236202451204; m[ 9][ 6] = 0.186848046932; m[ 9][ 7] = 0.189296292376; m[ 9][ 8] = 0.252718447885; m[ 9][ 9] = 0.000000000000; 
	m[ 9][10] = 3.890963773304; m[ 9][11] = 0.406193586642; m[ 9][12] = 3.364797763104; m[ 9][13] = 1.517359325954; m[ 9][14] = 0.388355409206; 
	m[ 9][15] = 0.357544412460; m[ 9][16] = 1.179091197260; m[ 9][17] = 0.341985787540; m[ 9][18] = 0.674617093228; m[ 9][19] = 8.311839405458; 
	m[10][ 0] = 0.800016530518; m[10][ 1] = 0.622711669692; m[10][ 2] = 0.211888159615; m[10][ 3] = 0.218131577594; m[10][ 4] = 0.831842640142; 
	m[10][ 5] = 0.580737093181; m[10][ 6] = 0.372625175087; m[10][ 7] = 0.217721159236; m[10][ 8] = 0.348072209797; m[10][ 9] = 3.890963773304; 
	m[10][10] = 0.000000000000; m[10][11] = 0.445570274261; m[10][12] = 6.030559379572; m[10][13] = 2.064839703237; m[10][14] = 0.374555687471; 
	m[10][15] = 0.352969184527; m[10][16] = 0.915259857694; m[10][17] = 0.691474634600; m[10][18] = 0.811245856323; m[10][19] = 2.231405688913; 
	m[11][ 0] = 1.295201266783; m[11][ 1] = 5.411115141489; m[11][ 2] = 1.593137043457; m[11][ 3] = 1.032447924952; m[11][ 4] = 0.285078800906; 
	m[11][ 5] = 3.945277674515; m[11][ 6] = 2.802427151679; m[11][ 7] = 0.752042440303; m[11][ 8] = 1.022507035889; m[11][ 9] = 0.406193586642; 
	m[11][10] = 0.445570274261; m[11][11] = 0.000000000000; m[11][12] = 1.073061184332; m[11][13] = 0.266924750511; m[11][14] = 1.047383450722; 
	m[11][15] = 1.752165917819; m[11][16] = 1.303875200799; m[11][17] = 0.332243040634; m[11][18] = 0.717993486900; m[11][19] = 0.498138475304; 
	m[12][ 0] = 1.253758266664; m[12][ 1] = 0.983692987457; m[12][ 2] = 0.648441278787; m[12][ 3] = 0.222621897958; m[12][ 4] = 0.767688823480; 
	m[12][ 5] = 2.494896077113; m[12][ 6] = 0.555415397470; m[12][ 7] = 0.459436173579; m[12][ 8] = 0.984311525359; m[12][ 9] = 3.364797763104; 
	m[12][10] = 6.030559379572; m[12][11] = 1.073061184332; m[12][12] = 0.000000000000; m[12][13] = 1.773855168830; m[12][14] = 0.454123625103; 
	m[12][15] = 0.918723415746; m[12][16] = 1.488548053722; m[12][17] = 0.888101098152; m[12][18] = 0.951682162246; m[12][19] = 2.575850755315; 
	m[13][ 0] = 0.492964679748; m[13][ 1] = 0.371644693209; m[13][ 2] = 0.354861249223; m[13][ 3] = 0.281730694207; m[13][ 4] = 0.441337471187; 
	m[13][ 5] = 0.144356959750; m[13][ 6] = 0.291409084165; m[13][ 7] = 0.368166464453; m[13][ 8] = 0.714533703928; m[13][ 9] = 1.517359325954; 
	m[13][10] = 2.064839703237; m[13][11] = 0.266924750511; m[13][12] = 1.773855168830; m[13][13] = 0.000000000000; m[13][14] = 0.233597909629; 
	m[13][15] = 0.540027644824; m[13][16] = 0.488206118793; m[13][17] = 2.074324893497; m[13][18] = 6.747260430801; m[13][19] = 0.838119610178; 
	m[14][ 0] = 1.173275900924; m[14][ 1] = 0.448133661718; m[14][ 2] = 0.494887043702; m[14][ 3] = 0.730628272998; m[14][ 4] = 0.356008498769; 
	m[14][ 5] = 0.858570575674; m[14][ 6] = 0.926563934846; m[14][ 7] = 0.504086599527; m[14][ 8] = 0.527007339151; m[14][ 9] = 0.388355409206; 
	m[14][10] = 0.374555687471; m[14][11] = 1.047383450722; m[14][12] = 0.454123625103; m[14][13] = 0.233597909629; m[14][14] = 0.000000000000; 
	m[14][15] = 1.169129577716; m[14][16] = 1.005451683149; m[14][17] = 0.252214830027; m[14][18] = 0.369405319355; m[14][19] = 0.496908410676; 
	m[15][ 0] = 4.325092687057; m[15][ 1] = 1.122783104210; m[15][ 2] = 2.904101656456; m[15][ 3] = 1.582754142065; m[15][ 4] = 1.197188415094; 
	m[15][ 5] = 1.934870924596; m[15][ 6] = 1.769893238937; m[15][ 7] = 1.509326253224; m[15][ 8] = 1.117029762910; m[15][ 9] = 0.357544412460; 
	m[15][10] = 0.352969184527; m[15][11] = 1.752165917819; m[15][12] = 0.918723415746; m[15][13] = 0.540027644824; m[15][14] = 1.169129577716; 
	m[15][15] = 0.000000000000; m[15][16] = 5.151556292270; m[15][17] = 0.387925622098; m[15][18] = 0.796751520761; m[15][19] = 0.561925457442; 
	m[16][ 0] = 1.729178019485; m[16][ 1] = 0.914665954563; m[16][ 2] = 1.898173634533; m[16][ 3] = 0.934187509431; m[16][ 4] = 1.119831358516; 
	m[16][ 5] = 1.277480294596; m[16][ 6] = 1.071097236007; m[16][ 7] = 0.641436011405; m[16][ 8] = 0.585407090225; m[16][ 9] = 1.179091197260; 
	m[16][10] = 0.915259857694; m[16][11] = 1.303875200799; m[16][12] = 1.488548053722; m[16][13] = 0.488206118793; m[16][14] = 1.005451683149; 
	m[16][15] = 5.151556292270; m[16][16] = 0.000000000000; m[16][17] = 0.513128126891; m[16][18] = 0.801010243199; m[16][19] = 2.253074051176; 
	m[17][ 0] = 0.465839367725; m[17][ 1] = 0.426382310122; m[17][ 2] = 0.191482046247; m[17][ 3] = 0.145345046279; m[17][ 4] = 0.527664418872; 
	m[17][ 5] = 0.758653808642; m[17][ 6] = 0.407635648938; m[17][ 7] = 0.508358924638; m[17][ 8] = 0.301248600780; m[17][ 9] = 0.341985787540; 
	m[17][10] = 0.691474634600; m[17][11] = 0.332243040634; m[17][12] = 0.888101098152; m[17][13] = 2.074324893497; m[17][14] = 0.252214830027; 
	m[17][15] = 0.387925622098; m[17][16] = 0.513128126891; m[17][17] = 0.000000000000; m[17][18] = 4.054419006558; m[17][19] = 0.266508731426; 
	m[18][ 0] = 0.718206697586; m[18][ 1] = 0.720517441216; m[18][ 2] = 0.538222519037; m[18][ 3] = 0.261422208965; m[18][ 4] = 0.470237733696; 
	m[18][ 5] = 0.958989742850; m[18][ 6] = 0.596719300346; m[18][ 7] = 0.308055737035; m[18][ 8] = 4.218953969389; m[18][ 9] = 0.674617093228; 
	m[18][10] = 0.811245856323; m[18][11] = 0.717993486900; m[18][12] = 0.951682162246; m[18][13] = 6.747260430801; m[18][14] = 0.369405319355; 
	m[18][15] = 0.796751520761; m[18][16] = 0.801010243199; m[18][17] = 4.054419006558; m[18][18] = 0.000000000000; m[18][19] = 1.000000000000; 
	m[19][ 0] = 2.187774522005; m[19][ 1] = 0.438388343772; m[19][ 2] = 0.312858797993; m[19][ 3] = 0.258129289418; m[19][ 4] = 1.116352478606; 
	m[19][ 5] = 0.530785790125; m[19][ 6] = 0.524253846338; m[19][ 7] = 0.253340790190; m[19][ 8] = 0.201555971750; m[19][ 9] = 8.311839405458; 
	m[19][10] = 2.231405688913; m[19][11] = 0.498138475304; m[19][12] = 2.575850755315; m[19][13] = 0.838119610178; m[19][14] = 0.496908410676; 
	m[19][15] = 0.561925457442; m[19][16] = 2.253074051176; m[19][17] = 0.266508731426; m[19][18] = 1.000000000000; m[19][19] = 0.000000000000; 	
    
	stationary_freqs[ 0] = 0.074; 
	stationary_freqs[ 1] = 0.052; 
	stationary_freqs[ 2] = 0.045; 
	stationary_freqs[ 3] = 0.054;
	stationary_freqs[ 4] = 0.025; 
	stationary_freqs[ 5] = 0.034; 
	stationary_freqs[ 6] = 0.054; 
	stationary_freqs[ 7] = 0.074;
	stationary_freqs[ 8] = 0.026; 
	stationary_freqs[ 9] = 0.068; 
	stationary_freqs[10] = 0.099; 
	stationary_freqs[11] = 0.058;
	stationary_freqs[12] = 0.025; 
	stationary_freqs[13] = 0.047; 
	stationary_freqs[14] = 0.039; 
	stationary_freqs[15] = 0.057;
	stationary_freqs[16] = 0.051; 
	stationary_freqs[17] = 0.013; 
	stationary_freqs[18] = 0.032; 
	stationary_freqs[19] = 0.073;

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
RateMatrix_Blosum62::~RateMatrix_Blosum62(void)
{
    
}




RateMatrix_Blosum62* RateMatrix_Blosum62::clone( void ) const
{

    return new RateMatrix_Blosum62( *this );
}


