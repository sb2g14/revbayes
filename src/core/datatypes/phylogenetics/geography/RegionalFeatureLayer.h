//
//  RegionalFeatureLayer.hpp
//  revbayes-tensorphylo-proj
//
//  Created by Michael Landis on 7/10/22.
//  Copyright © 2022 Michael Landis. All rights reserved.
//

#ifndef RegionalFeatureLayer_hpp
#define RegionalFeatureLayer_hpp

#include "Cloneable.h"
#include <vector>

namespace RevBayesCore {

    class RegionalFeatureLayer : public Cloneable
    {
    public:
        
        RegionalFeatureLayer();
        RegionalFeatureLayer(size_t ti, size_t fi, std::string fr, std::string ft);
        RegionalFeatureLayer(const RegionalFeatureLayer& g);
        RegionalFeatureLayer&               operator=(const RegionalFeatureLayer& g);
        virtual RegionalFeatureLayer*       clone(void) const;
        
        bool                                operator==(const RegionalFeatureLayer &t) const;           //!< Equals operators
        bool                                operator!=(const RegionalFeatureLayer &t) const;           //!< Not-quals operators
        bool                                operator<(const RegionalFeatureLayer &t) const;            //!< Less-than operators
        bool                                operator<=(const RegionalFeatureLayer &t) const;           //!< Less-than operators
        bool                                operator>(const RegionalFeatureLayer &t) const;            //!< Less-than operators
        bool                                operator>=(const RegionalFeatureLayer &t) const;           //!< Less-than operators
        
        std::vector<long>                   getWithinCategoricalFeatures(void) const;
        std::vector<double>                 getWithinQuantitativeFeatures(void) const;
        std::vector<std::vector<long> >     getBetweenCategoricalFeatures(void) const;
        std::vector<std::vector<double> >   getBetweenQuantitativeFeatures(void) const;
        size_t                              getTimeIndex(void) const;
        size_t                              getFeatureIndex(void) const;
        std::string                         getFeatureType(void) const;
        std::string                         getFeatureRelationship(void) const;
    
        void                                setFeatures(std::vector<long> f);
        void                                setFeatures(std::vector<double> f);
        void                                setFeatures(std::vector<std::vector<long> > f);
        void                                setFeatures(std::vector<std::vector<double> > f);
        
    private:
        size_t                              time_index;
        size_t                              feature_index;
        std::string                         feature_type;
        std::string                         feature_relationship;
        
        // this could be a template
        std::vector<long>                   within_categorical;
        std::vector<double>                 within_quantitative;
        std::vector<std::vector<long> >     between_categorical;
        std::vector<std::vector<double> >   between_quantitative;
    };

    std::ostream& operator<<(std::ostream& o, const RegionalFeatureLayer& x);
}

#endif /* RegionalFeatureLayer_hpp */
