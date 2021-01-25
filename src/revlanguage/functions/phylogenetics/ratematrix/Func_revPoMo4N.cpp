#include "Func_revPoMo4N.h"
#include "revPoMo4NRateMatrixFunction.h"
#include "RateMatrix_revPoMo4N.h"
#include "ModelVector.h"
#include "Natural.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_revPoMo4N::Func_revPoMo4N( void ) : TypedFunction<RateMatrix>( )
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_revPoMo4N* Func_revPoMo4N::clone( void ) const
{
    
    return new Func_revPoMo4N( *this );
}


RevBayesCore::TypedFunction< RevBayesCore::RateGenerator >* Func_revPoMo4N::createFunction( void ) const
{
    RevBayesCore::TypedDagNode< long                          >* ni = static_cast<const Natural              &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<RevBayesCore::Simplex          >* bf = static_cast<const Simplex              &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* ex = static_cast<const ModelVector<RealPos> &>( this->args[2].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* fc = static_cast<const ModelVector<RealPos> &>( this->args[3].getVariable()->getRevObject() ).getDagNode();

    if ( ex->getValue().size() != 6 )
    {
        throw RbException("The number of exchangeabilities should match the number of biallelic polymorphic types: 6.");
    }
    if ( bf->getValue().size() != 4 )
    {
        throw RbException("The number of allele frequencies should match the number of alleles: 4.");
    }
    if ( fc->getValue().size() != 4 )
    {
        throw RbException("The number of fitness coeffiients should match the number of alleles: 4.");
    }

    RevBayesCore::revPoMo4NRateMatrixFunction* f = new RevBayesCore::revPoMo4NRateMatrixFunction( ni, bf, ex, fc );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_revPoMo4N::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rules_set = false;
    
    if ( !rules_set )
    {
        argumentRules.push_back( new ArgumentRule( "N"   , Natural::getClassTypeSpec(), "Number of individuals.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "pi"  , Simplex::getClassTypeSpec(), "Vector of allele frequencies.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "rho" , ModelVector<RealPos>::getClassTypeSpec(), "Vector of mutation rates: mu=(mu_a0a1,mu_a1a0,mu_a0a2,...).", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "phi" , ModelVector<RealPos>::getClassTypeSpec(), "Vector of fitness coefficients: phi=(phi_0,phi_1,...).", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );  

        rules_set = true;
    }
    
    return argumentRules;
}


const std::string& Func_revPoMo4N::getClassType(void)
{
    
    static std::string rev_type = "Func_revPoMo4N";
    
    return rev_type;
}


/* Get class type spec describing type of object */
const TypeSpec& Func_revPoMo4N::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return rev_type_spec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_revPoMo4N::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "fnReversiblePoMo4N";
    
    return f_name;
}


const TypeSpec& Func_revPoMo4N::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}


