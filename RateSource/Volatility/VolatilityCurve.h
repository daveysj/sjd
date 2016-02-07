#ifndef SJD_VOLATILITYCURVE_INCLUDED
#define SJD_VOLATILITYCURVE_INCLUDED
#pragma once

#include <sjd\Tools\DatedCurve.h>
#include <sjd\RateSource\Volatility\DeterministicVolatilityRateSource.h>

#include <ql\time\daycounters\simpledaycounter.hpp>
#include <ql\time\calendars\nullcalendar.hpp>

using namespace QuantLib;

namespace sjd 
{

    /*======================================================================================
    VolatilityCurve

    Use this for European Volatilities observable on specific dates.
    NOTE 1) Interpolation happens in the Standard Deviation Space and not the Volatility 
            Space
         2) There are no calendar objects here, all dates are assumed to be good dates
            by the time they hit this object
    =======================================================================================*/
    class VolatilityCurve : public DeterministicVolatilityRateSource
    {
    public:
        
        VolatilityCurve(Date anchorDate, 
                        vector<Date> observationDates, 
                        vector<double> volatility, 
                        boost::shared_ptr<DayCounter> dayCounter,
                        ArrayInterpolatorType type = LINEAR,
                        bool allowExtrapolation = false);    
        
        virtual ~VolatilityCurve();

        virtual bool isOK();
        virtual vector<Date> getObservationDates();

        virtual bool isInRange(Date date) const;
        virtual bool isInRange(Date date, double strike, double forward) const;

        virtual double getVolatility(Date toDate) const;
        virtual double getVolatility(Date toDate, double strike, double forward) const;

        virtual double getStandardDeviation(Date toDate) const;
        virtual double getStandardDeviation(Date toDate, double strike, double forward) const;

        virtual void getVolAndSD(Date toDate, double &vol, double& sd) const;
        virtual void getVolAndSD(Date toDate, double strike, double forward, double &vol, double& sd) const;

        vector<Date> getObservationDates() const;
        ArrayInterpolatorType getType();

        virtual boost::shared_ptr<VolatilityRateSource> parallelBump(double spread);
        virtual boost::shared_ptr<VolatilityRateSource> rollForward(Date toDate);

    private:
        boost::shared_ptr<DatedCurve> datedCurve;
        ArrayInterpolatorType type;
    };
}

#endif