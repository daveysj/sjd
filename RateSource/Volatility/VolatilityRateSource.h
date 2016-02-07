#ifndef SJD_VOLATILITYRATESOURCE_INCLUDED
#define SJD_VOLATILITYRATESOURCE_INCLUDED
#pragma once

#include <vector>
#include <sjd\RateSource\RateSource.h>

using namespace std;
using namespace QuantLib;

namespace sjd 
{
   /*======================================================================================
   VolatlityRateSource

    Any object which can be queried for a standard deviation (the preferred unit to work with
    because it is a unitless measure) on future dates must derive from this base class. 
    Examples include
        Volatility Curves
        Volatility Strike or Moneyness Surfaces 
        Volatility Delta Surfaces
    =======================================================================================*/
    class VolatilityRateSource : public RateSource
    {
    public:
        virtual ~VolatilityRateSource() {};

        virtual bool isOK();

        boost::shared_ptr<QuantLib::DayCounter> getDayCounter() const;

        using RateSource::isInRange;
        virtual bool isInRange(Date date, double strike, double forward) const = 0;

        // check to see if we can roll along the curve using "spot vol"
        bool hasNegativeATMFwdFwdVariances(Date &startOfNegVar, Date &endOfNegVar);
        virtual vector<Date> getObservationDates() = 0;
        bool hasNegativeATMFwdFwdVariances(vector<Date> checkDates, Date &startOfNegVar, Date &endOfNegVar);

        // NB Dates assumed to be good. Call isInRange(...) before calling this
        virtual double getVolatility(Date toDate) const = 0;
        virtual double getVolatility(Date toDate, double strike, double forward) const = 0;
        virtual double getStandardDeviation(Date toDate) const;
        virtual double getStandardDeviation(Date toDate, double strike, double forward) const;
        virtual void getVolAndSD(Date toDate, double &vol, double& sd) const;
        virtual void getVolAndSD(Date toDate, double strike, double forward, double &vol, double& sd) const;

        // Ensure the input vols and sds vectors are of the correct size before using this method
        virtual void getVolAndSD(vector<Date> toDates, 
                                 double strike, 
                                 vector<double> forwards, 
                                 vector<double> &vols, 
                                 vector<double> &sds) const;
    
    protected:
        boost::shared_ptr<QuantLib::DayCounter> dayCounter;
    };

}

#endif