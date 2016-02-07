#ifndef SJD_CURVE_INCLUDED
#define SJD_CURVE_INCLUDED

#include <sjd\maths\maths.h>
#include <ql\time\date.hpp>
#include <boost\shared_ptr.hpp>

using namespace QuantLib;

namespace sjd 
{
   /*======================================================================================
    Curve: 

    NOTE 1) There are no calendar objects here, all dates are assumed to be good dates
            by the time they hit this class
    =======================================================================================*/
    class Curve
    {
    public:
        Curve();
        
        Curve(Date anchorDate, 
              vector<double> observationPoints, 
              vector<double> observedRates, 
              ArrayInterpolatorType type = LINEAR,
              bool allowExtrapolation = false);  

        void setParameters(Date anchorDate, 
                           vector<double> observationPoints, 
                           vector<double> observedRates, 
                           ArrayInterpolatorType type = LINEAR,
                           bool allowExtrapolation = false);
        
        virtual ~Curve() {};

        bool isOK(string &errorMessage);

        Date getAnchorDate();
        Date getFinalDate();
        ArrayInterpolatorType getType();
        bool getAllowsExtrapolation();

        vector<double> getRates();
        void setRates(vector<double> updatedRates);

        bool containsRateOnAnchorDate();
        void addPointOnAnchorDate(double valueToAdd);
        double getRate(double toPoint) const;

        vector<double> getObservationPoints() const;


    protected:
        Date anchorDate;
        vector<double> observationPoints;
        vector<double> observedRates;
        ArrayInterpolatorType type;
        boost::shared_ptr<ArrayInterpolator> interpolator;
        bool allowExtrapolation;
    };

}

#endif