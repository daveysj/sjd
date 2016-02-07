#ifndef SJD_VOLATILITYCURVECMA_INCLUDED
#define SJD_VOLATILITYCURVECMA_INCLUDED
#pragma once

#include <sjd\Tools\CalendarMonthAverageCurve.h>
#include <sjd\RateSource\Volatility\DeterministicVolatilityRateSource.h>

#include <ql\time\daycounters\simpledaycounter.hpp>
#include <ql\time\calendars\nullcalendar.hpp>

using namespace QuantLib;

namespace sjd 
{
    /*======================================================================================
    VolatilityCurveCMA

    Use this for volatility curves where every date in a single calendar month has the same 
    volatility
    =======================================================================================*/
    class VolatilityCurveCMA : public DeterministicVolatilityRateSource
    {
    public:
        
        VolatilityCurveCMA(Date anchorDate, 
                        vector<Date> observationDates, 
                        vector<double> volatility, 
                        boost::shared_ptr<DayCounter> dayCounter,
                        ArrayInterpolatorType type = LINEAR,
                        bool allowExtrapolation = false);    
        
        virtual ~VolatilityCurveCMA();

        virtual bool isOK();
        virtual vector<Date> getObservationDates();

        virtual bool isInRange(Date date) const;
        virtual bool isInRange(Date date, double strike, double forward) const;
        virtual bool extendsTo(Date date) const;        

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
        boost::shared_ptr<CalendarMonthAverageCurve> curve;
        ArrayInterpolatorType type;
    };
}

#endif