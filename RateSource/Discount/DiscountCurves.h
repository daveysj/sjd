#ifndef SJD_DISCOUNTCURVE_INCLUDED
#define SJD_DISCOUNTCURVE_INCLUDED

#include <sjd\Tools\DatedCurve.h>
#include <sjd\RateSource\Discount\DeterministicDiscountRateSource.h>
#include <sjd\RateSource\Discount\InterestRateConvention.h>

#include <ql\time\calendars\nullcalendar.hpp>

using namespace QuantLib;

namespace sjd 
{
   /*======================================================================================
    DiscountCurve: The simplest DiscountRateSource 

    NOTE : There are no calendar objects here, all dates are assumed to be good dates
            by the time they hit this class
    =======================================================================================*/
    class DiscountCurve : public DeterministicDiscountRateSource
    {
    public:
        DiscountCurve();
        
        DiscountCurve(Date anchorDate, 
                      vector<Date> observationDates, 
                      vector<double> discountFactors, 
                      ArrayInterpolatorType type = LINEAR,
                      bool allowExtrapolation = false);    
        
        virtual ~DiscountCurve() {};

        virtual bool isOK();

        virtual double getDiscountFactor(Date toDate) const;
        using DiscountRateSource::getDiscountFactor;

        vector<Date> getObservationDates() const;
        ArrayInterpolatorType getType();

        virtual vector<pair<string, boost::shared_ptr<RateSource>>> getMarketRateStresses();
        virtual boost::shared_ptr<DiscountRateSource> parallelBump(double spread,
                                                                   boost::shared_ptr<InterestRateConvention> irc);

        virtual boost::shared_ptr<DiscountRateSource> rollForward(Date toDate);

    protected:
        boost::shared_ptr<DatedCurve> datedCurve;
        ArrayInterpolatorType type;
    };
}

#endif