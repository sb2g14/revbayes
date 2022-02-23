#include <stddef.h>
#include <algorithm>
#include <cmath>
#include <iosfwd>
#include <set>
#include <string>
#include <vector>

#include "PiecewiseConstantCoalescent.h"
#include "DistributionExponential.h"
#include "RandomNumberFactory.h"
#include "RbConstants.h"
#include "TopologyNode.h"
#include "AbstractCoalescent.h"
#include "RbException.h"
#include "RbVector.h"
#include "RbVectorImpl.h"
#include "Tree.h"
#include "TypedDagNode.h"

namespace RevBayesCore { class Clade; }
namespace RevBayesCore { class DagNode; }
namespace RevBayesCore { class RandomNumberGenerator; }
namespace RevBayesCore { class Taxon; }

using namespace RevBayesCore;

PiecewiseConstantCoalescent::PiecewiseConstantCoalescent(const TypedDagNode<RbVector<double> > *N, const TypedDagNode<RbVector<double> > *i, METHOD_TYPES meth, const std::vector<Taxon> &tn, const std::vector<Clade> &c) :
    AbstractCoalescent( tn, c ),
    Nes( N ),
    interval_change_points_var( i ),
    interval_method( meth )
{
    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    addParameter( Nes );
    addParameter( interval_change_points_var );
    
    simulateTree();
    
    updateIntervals();
}



PiecewiseConstantCoalescent::~PiecewiseConstantCoalescent()
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of myself
 */
PiecewiseConstantCoalescent* PiecewiseConstantCoalescent::clone( void ) const
{
    
    return new PiecewiseConstantCoalescent( *this );
}


/**
 * Compute the log-transformed probability of the current times under the current parameter values.
 *
 * \return    The log-probability density.
 */
double PiecewiseConstantCoalescent::computeLnProbabilityTimes( void ) const
{
    
    // variable declarations and initialization
    double ln_prob = 0;
    
    // update the interval change points and coalescent sizes
    updateIntervals();
    
    // retrieved the coalescent ages
    std::vector<double> coalescent_ages;
    for (size_t i = 0; i < value->getNumberOfInteriorNodes()+1; ++i)
    {
        const TopologyNode& n = value->getInteriorNode( i );
        double a = n.getAge();
        coalescent_ages.push_back(a);
    }
    // sort the vector of ages in ascending order
    std::sort(coalescent_ages.begin(), coalescent_ages.end());
    
    // retrieve the ages of any serially sampled tips
    std::vector<double> serial_tip_ages;
    size_t num_taxa_at_present = value->getNumberOfTips();
    for (size_t i = 0; i < value->getNumberOfTips(); ++i)
    {
        const TopologyNode& n = value->getTipNode( i );
        double a = n.getAge();
        if ( a > 0.0 ) {
            serial_tip_ages.push_back(a);
            --num_taxa_at_present;
        }
    }
    
    std::vector<double> combined_event_ages;
    std::vector<EVENT_TYPE> combined_event_types;

    // sort the vector of serial sampling times in ascending order
    std::sort(serial_tip_ages.begin(), serial_tip_ages.end());

    // initialize variables
    size_t at_coal_age                  = 0;
    size_t at_serial_age                = 0;
    size_t at_interval_change_point     = 0;
    double next_coal_age                = coalescent_ages[at_coal_age];
    double next_interval_change_point   = ( interval_change_points.size() > 0 ? interval_change_points[at_interval_change_point] : RbConstants::Double::inf );
    // set the next serially sampled tip age if present, otherwise Inf
    double next_serial_age              = (at_serial_age < serial_tip_ages.size() ? serial_tip_ages[at_serial_age] : RbConstants::Double::inf);

    // create master list of event times and types
    // events are either a sample (lineage size up), coalescence (lineage size down), or theta changepoint (lineage size constant)
    do
    {
        next_coal_age = coalescent_ages[at_coal_age];
        if (next_coal_age <= next_serial_age && next_coal_age <= next_interval_change_point)
        {
            // coalescence
            combined_event_ages.push_back( next_coal_age );
            combined_event_types.push_back( COALESCENT );
            ++at_coal_age;
        }
        else if ( next_serial_age <= next_coal_age && next_serial_age <= next_interval_change_point )
        {
            // serial sample
            combined_event_ages.push_back( next_serial_age );
            combined_event_types.push_back( SERIAL_SAMPLE );
            ++at_serial_age;
            // set the next serial age
            if (at_serial_age < serial_tip_ages.size())
            {
                next_serial_age = serial_tip_ages[at_serial_age];
            }
            else
            {
                next_serial_age = RbConstants::Double::inf;
            }
        }
        else
        {
            // theta change
            combined_event_ages.push_back( next_interval_change_point );
            combined_event_types.push_back( DEMOGRAPHIC_MODEL_CHANGE );
            ++at_interval_change_point;
            if ( at_interval_change_point < interval_change_points.size() )
            {
                next_interval_change_point = interval_change_points[at_interval_change_point];
            }
            else
            {
                next_interval_change_point = RbConstants::Double::inf;
            }
        }
    } while (at_coal_age < coalescent_ages.size());


    size_t current_interval = 0;
    size_t current_num_lineages = num_taxa_at_present;
    double last_event_age = 0.0;

    for (size_t i = 0; i < combined_event_ages.size(); ++i)
    {
        
        double theta = pop_sizes[current_interval];
        double n_pairs = current_num_lineages * (current_num_lineages-1) / 2.0;
        
        double delta_age = combined_event_ages[i] - last_event_age;

        if (combined_event_types[i] == COALESCENT)
        {
            // coalescence
            ln_prob += log( 1.0 / theta ) - n_pairs * delta_age / theta;
            --current_num_lineages;
        }
        else if (combined_event_types[i] == SERIAL_SAMPLE)
        {
            // sampled ancestor
            ln_prob -= n_pairs * delta_age / theta ;
            ++current_num_lineages;
        }
        else
        {
            // theta change
            ln_prob -= n_pairs * delta_age / theta ;
            ++current_interval;
        }

        last_event_age = combined_event_ages[i];
    }

    return ln_prob;
}


void PiecewiseConstantCoalescent::executeMethod(const std::string &n, const std::vector<const DagNode *> &args, RbVector<double> &rv) const
{
    
    if ( n == "getIntervalAges" )
    {
        
        rv = interval_change_points;
        
    }
    else
    {
        throw RbException("The piecewise-constant coalescent process does not have a member method called '" + n + "'.");
    }
    
}


/**
 * Keep the current value and reset some internal flags. Nothing to do here.
 */
void PiecewiseConstantCoalescent::keepSpecialization(const DagNode *affecter)
{
    
    // nothing to do here
    
}

/**
 * Restore the current value and reset some internal flags.
 * If the root age variable has been restored, then we need to change the root age of the tree too.
 */
void PiecewiseConstantCoalescent::restoreSpecialization(const DagNode *affecter)
{

    // Sebastian: This is currently redudant because we update the intervals each time when we compute the probability
    // just re-update the start times of the intervals
//    updateIntervals();
    
}

//
///**
// * Simulate new coalescent times.
// *
// * \param[in]    n      The number of coalescent events to simulate.
// *
// * \return    A vector of the simulated coalescent times.
// */
//std::vector<double> PiecewiseConstantCoalescent::simulateCoalescentAges( size_t n ) const
//{
//    
//    // first check if we want to set the interval ages here!
//    if ( interval_method == SPECIFIED )
//    {
//        updateIntervals();
//    }
//    else
//    {
//        interval_change_points = RbVector<double>(Nes->getValue().size()-1, RbConstants::Double::inf);
//    }
//    size_t num_events_per_interval = size_t( floor( double(n)/Nes->getValue().size()) );
//    size_t current_num_events_in_interval = 0;
//    
//    // Get the rng
//    RandomNumberGenerator* rng = GLOBAL_RNG;
//    
//    // allocate the vector for the times
//    std::vector<double> coalescent_ages = std::vector<double>(n,0.0);
//    
//    const RbVector<double> &pop_sizes  = Nes->getValue();
//    size_t current_interval = 0;
//    size_t current_num_lineages = num_taxa;
//    // draw a time for each speciation event condition on the time of the process
//    for (size_t i = 0; i < n; ++i)
//    {
//        double prev_coalescent_age = 0.0;
//        if ( i > 0 )
//        {
//            prev_coalescent_age = coalescent_ages[i-1];
//        }
//        
//        double n_pairs = current_num_lineages * (current_num_lineages-1) / 2.0;
//        
//        double sim_age = 0.0;
//        bool was_coalescent_event = false;
//        do
//        {
//            double theta = 1.0 / (pop_sizes[current_interval]);
//            double lambda = n_pairs * theta;
//            double u = RbStatistics::Exponential::rv( lambda, *rng);
//            sim_age = prev_coalescent_age + u;
//            was_coalescent_event = current_interval >= interval_change_points.size() || sim_age < interval_change_points[current_interval];
//            if ( was_coalescent_event == false )
//            {
//                prev_coalescent_age = interval_change_points[current_interval];
//                ++current_interval;
//            }
//            
//        } while ( was_coalescent_event == false );
//        
//        coalescent_ages[i] = sim_age;
//        
//        ++current_num_events_in_interval;
//        if ( interval_method == EVENTS && current_num_events_in_interval == num_events_per_interval && current_interval < interval_change_points.size() )
//        {
//            interval_change_points[current_interval] = sim_age;
//            current_num_events_in_interval = 0;
//        }
//        
//        --current_num_lineages;
//    }
//    
//    return coalescent_ages;
//}



/**
 * Simulate new coalescent times.
 *
 * \param[in]    n      The number of coalescent events to simulate.
 *
 * \return    A vector of the simulated coalescent times.
 */
std::vector<double> PiecewiseConstantCoalescent::simulateCoalescentAges( size_t n ) const
{
    
    // first check if we want to set the interval ages here!
    if ( interval_method == SPECIFIED )
    {
        updateIntervals();
    }
    else
    {
        interval_change_points = RbVector<double>(Nes->getValue().size()-1, RbConstants::Double::inf);
        pop_sizes = Nes->getValue();
    }

    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // retrieve the times of any serially sampled tips
    std::vector<double> serial_tip_ages;
    size_t num_taxa_at_present = 0;
    for (size_t i = 0; i < num_taxa; ++i)
    {
        double a = taxa[i].getAge();
        if ( a > 0.0 )
        {
            serial_tip_ages.push_back(a);
        }
        else
        {
            ++num_taxa_at_present;
        }
    }

    
    // Put sampling times and pop-size changes into a single vector of event times
    std::vector<double>     combined_event_ages;
    std::vector<EVENT_TYPE> combined_event_types;
    
    // sort the vector of serial sampling times in ascending order
    std::sort(serial_tip_ages.begin(), serial_tip_ages.end());
    
    // initialize variables
    size_t at_coal_age                  = 0;
    size_t at_serial_age                = 0;
    size_t at_interval_change_point     = 0;
    double next_interval_change_point   = interval_change_points[at_interval_change_point];
    // set the next serially sampled tip age if present, otherwise Inf
    double next_serial_age              = (at_serial_age < serial_tip_ages.size() ? serial_tip_ages[at_serial_age] : RbConstants::Double::inf);

    
    // create master list of event times and types
    // pre-defined events are either a sample (lineage size up) or theta changepoint (lineage size constant)
    size_t num_total_events = ( interval_method == SPECIFIED ? interval_change_points.size() + serial_tip_ages.size() : serial_tip_ages.size() );
    for (size_t num_events = 0; num_events < num_total_events; ++num_events)
    {
        if (next_interval_change_point <= next_serial_age)
        {
            // theta change
            combined_event_ages.push_back( next_interval_change_point );
            combined_event_types.push_back( DEMOGRAPHIC_MODEL_CHANGE);
            ++at_interval_change_point;
            if (at_interval_change_point < interval_change_points.size())
            {
                next_interval_change_point = interval_change_points[at_interval_change_point];
            }
            else
            {
                next_interval_change_point = RbConstants::Double::inf;
            }
        }
        else
        {
            // serial sample
            combined_event_ages.push_back(next_serial_age);
            combined_event_types.push_back( SERIAL_SAMPLE );
            ++at_serial_age;
            if (at_serial_age < serial_tip_ages.size())
            {
                next_serial_age = serial_tip_ages[at_serial_age];
            }
            else
            {
                next_serial_age = RbConstants::Double::inf;
            }
        }
    }
//    } else {
//        combinedEventTimes = intervals;
//        combinedEventTypes = std::vector<double>(intervals.size(),0.0);
//    }
 
//    // cap vector with an event at t-infinity
//    combinedEventTimes.push_back(RbConstants::Double::inf);
//    combinedEventTypes.push_back(RbConstants::Double::inf);
    
    if ( interval_method == EVENTS )
    {
        // add dummies for the change point times
        for ( size_t i=0; i<interval_change_points.size(); ++i)
        {
            combined_event_ages.push_back( RbConstants::Double::inf );
            combined_event_types.push_back( DEMOGRAPHIC_MODEL_CHANGE );
        }
    }

    // allocate the vector for the times
    std::vector<double> coalescent_ages = std::vector<double>(n,0.0);
    
    size_t current_interval = 0;
    size_t theta_interval = 0;
    size_t current_num_lineages = num_taxa_at_present;
    
    size_t num_events_per_interval = size_t( ceil( double(n)/Nes->getValue().size()) );
    size_t current_num_events_in_interval = 0;
    
    // the current age of the process
    double sim_age = 0.0;

    // now simulate the ages
    for (size_t i = 0; i < n; ++i)
    {
        
        bool was_coalescent_event = false;
        do
        {
            double n_pairs = current_num_lineages * (current_num_lineages-1) / 2.0;
            double theta = pop_sizes[theta_interval];
            double lambda = n_pairs / theta;
            double u = RbStatistics::Exponential::rv( lambda, *rng );
            sim_age += u;
            was_coalescent_event = sim_age < combined_event_ages[current_interval] && current_num_lineages > 1;
            if ( was_coalescent_event == false )
            {
                // If j is 1 and we are still simulating coalescent events, we have >= 1 serial sample left to coalesce.
                // There are no samples to coalesce now, but we cannot exit, thus, we advance to the next serial sample
                // Alternately, when we cross a serial sampling time or theta window, the number of active lineages changes
                // or the pop size changes, and it is necessary to discard any "excess" time,
                // which is drawn from an incorrect distribution,then we can draw a new time according to
                // the correct number of active lineages.
                // Either we advance or go back, but in both situations we set the time to the current event in combinedEvents.
                sim_age = combined_event_ages[current_interval];
                if (combined_event_types[current_interval] == DEMOGRAPHIC_MODEL_CHANGE)
                {
                    // theta change
                    ++theta_interval;
                }
                else
                {
                    // serial sample
                    ++current_num_lineages;
                }
                ++current_interval;
            }
        } while ( was_coalescent_event == false );

        coalescent_ages[i] = sim_age;
        
        ++current_num_events_in_interval;
        if ( interval_method == EVENTS && current_num_events_in_interval == num_events_per_interval && theta_interval < interval_change_points.size() )
        {
            interval_change_points[theta_interval] = sim_age;
            current_num_events_in_interval = 0;
            ++theta_interval;
        }
        
        --current_num_lineages;
    }
    
    return coalescent_ages;
}


/**
 * Swap the parameters held by this distribution.
 *
 *
 * \param[in]    old_p      Pointer to the old parameter.
 * \param[in]    new_p      Pointer to the new parameter.
 */
void PiecewiseConstantCoalescent::swapParameterInternal(const DagNode *old_p, const DagNode *new_p)
{
    if (old_p == Nes)
    {
        Nes = static_cast<const TypedDagNode<RbVector<double> >* >( new_p );
    }
    else if (old_p == interval_change_points_var)
    {
        interval_change_points_var = static_cast<const TypedDagNode<RbVector<double> >* >( new_p );
    }
}



/**
 * Touch the current value and reset some internal flags.
 * If the root age variable has been restored, then we need to change the root age of the tree too.
 */
void PiecewiseConstantCoalescent::touchSpecialization(const DagNode *affecter, bool touchAll)
{
    // Sebastian: This is currently redudant because we update the intervals each time when we compute the probability
    // just update the start times of the intervals
//    updateIntervals();
    
}



/**
 * Recompute the current interval change point vector and corresponding population size vector.
 *
 */
void PiecewiseConstantCoalescent::updateIntervals( void ) const
{
    
    // first check if we want to set the interval ages here!
    if ( interval_method == SPECIFIED )
    {
        // we obtained the interval ages, so we just need to make sure that they are sorted.
        if ( interval_change_points_var == NULL )
        {
            throw RbException("You have to provide the start times of the coalescent skyline intervals if you chose 'SPECIFIED' as the method of choice.");
        }
        
        // clean all the sets
        interval_change_points.clear();
        pop_sizes.clear();
                
        // important, we use this set of ages to automatically sort the ages.
        std::set<double> event_ages;
        
        const std::vector<double>& change_point_ages = interval_change_points_var->getValue();
        for (std::vector<double>::const_iterator it = change_point_ages.begin(); it != change_point_ages.end(); ++it)
        {
            event_ages.insert( *it );
        }
        
        // get our current population sizes
        const std::vector<double> &p = Nes->getValue();
                
        // we assume that the first population size corresponds to the interval starting at time 0
        pop_sizes.push_back( p[0] );
        
        size_t index = 0;
        size_t pos = 0;
        for (std::set<double>::iterator it = event_ages.begin(); it != event_ages.end(); ++it)
        {
            // get the age for the event
            double a = *it;
            
            // add the time to our vector
            interval_change_points.push_back( a );
            
            // add the population size at the event a
            pos = size_t( find(change_point_ages.begin(), change_point_ages.end(), a) - change_point_ages.begin() );
            if ( pos != change_point_ages.size() )
            {
                index = pos;
                pop_sizes.push_back( p[index+1] );
            }
            else
            {
                throw RbException("Couldn't sort vector of interval change points.");
            }
            
        }
        
        
    }
    else if ( interval_method == EVENTS )
    {
        // we directly use the population size given from the arguments
        pop_sizes = Nes->getValue();
        
        // next, we recompute the starting times of new intervals
        interval_change_points = RbVector<double>(Nes->getValue().size()-1, RbConstants::Double::inf);
        
        if ( this->value != NULL )
        {            
            // retrieve the coalescent ages
            std::vector<double> coalescent_ages;
            for (size_t i = 0; i < value->getNumberOfInteriorNodes()+1; ++i)
            {
                const TopologyNode& n = value->getInteriorNode( i );
                double a = n.getAge();
                coalescent_ages.push_back(a);
            }
            // sort the vector of times in ascending order
            std::sort(coalescent_ages.begin(), coalescent_ages.end());
            
            size_t num_events_per_interval = size_t( ceil( double(num_taxa-1.0)/Nes->getValue().size()) );
            size_t current_interval = 0;
            size_t current_num_events_in_interval = 0;
            for (size_t i = 0; i < num_taxa-2; ++i)
            {
                ++current_num_events_in_interval;
                if ( current_num_events_in_interval == num_events_per_interval )
                {
                    interval_change_points[current_interval] = coalescent_ages[i];
                    current_num_events_in_interval = 0;
                    ++current_interval;
                }
            }
        }

    }
    
}
