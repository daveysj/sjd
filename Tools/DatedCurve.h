#ifndef SJD_DATEDCURVE_INCLUDED
#define SJD_DATEDCURVE_INCLUDED

#include <sjd\maths\maths.h>
#include <ql\time\date.hpp>
#include <boost\shared_ptr.hpp>

using namespace QuantLib;

namespace sjd 
{
   /*======================================================================================
    DatedCurve: 

    NOTE 1) There are no calendar objects here, all dates are assumed to be good dates
            by the time they hit this class
    =======================================================================================*/
    class DatedCurve
    {
    public:
        DatedCurve();
        
        DatedCurve(Date anchorDate, 
                   vector<Date> observationDates, 
                   vector<double> rates, 
                   ArrayInterpolatorType type = LINEAR,
                   bool allowExtrapolation = false);    
        
        void setParameters(Date anchorDate, 
                           vector<Date> observationDates, 
                           vector<double> rates, 
                           ArrayInterpolatorType type = LINEAR,
                           bool allowExtrapolation = false);    

        virtual ~DatedCurve() {};        

        bool isOK(string &errorMessage);

        Date getAnchorDate();
        Date getFinalDate();
        ArrayInterpolatorType getType();
        bool getAllowsExtrapolation();

        vector<double> getRates();
        void setRates(vector<double> updatedRates);

        bool containsRateOnAnchorDate();
        void addPointOnAnchorDate(double valueToAdd);

        double getRate(Date toDate) const;

        //vector<double> getObservationPoints() const;
        vector<Date> getObservationDates() const;

    protected:
        Date anchorDate;
        Date finalDate;
        vector<Date> observationDates;
        vector<double> observationPoints;
        vector<double> observedRates;
        ArrayInterpolatorType type;
        boost::shared_ptr<ArrayInterpolator> interpolator;
        bool allowExtrapolation;
    };

}

#endif