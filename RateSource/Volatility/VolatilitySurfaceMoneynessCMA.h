
#ifndef SJD_CALENDARMONTHMONEYNESS_INCLUDED
#define SJD_CALENDARMONTHMONEYNESS_INCLUDED

#include "VolatilitySurfaces.h"


using namespace QuantLib;


namespace sjd 
{
    /*======================================================================================
    VolatilitySurfaceMoneynessCMA 

    This class replicates how we are using the Calendar Month Average Volatility in 
    Prototrader. There is no provision for extrapolation in either the time or moneyness
    dimensions. 

    Note: rolling forward in time
    =======================================================================================*/
    class VolatilitySurfaceMoneynessCMA : public VolatiltiySurfaceRateSource
    {
    public:
        
        VolatilitySurfaceMoneynessCMA(Date anchorDate, 
                                      vector<Date> observationDates, 
                                      vector<double> moneyness,
                                      QuantLib::Matrix volatilityData,
                                      SurfaceInterpolatorType interpolatorType,
                                      bool extrapolate = true); // row = moneyness, column = time
        
        VolatilitySurfaceMoneynessCMA(Date anchorDate, 
                                      vector<Date> observationDates, 
                                      vector<double> moneyness,
                                      vector<vector<double>> volatilityData,
                                      SurfaceInterpolatorType interpolatorType,
                                      bool extrapolate = true); // row = moneyness, column = time

        virtual ~VolatilitySurfaceMoneynessCMA() {};

        virtual void setInputs(Date anchorDate, 
                               vector<Date> observationDates, 
                               vector<double> moneyness,
                               QuantLib::Matrix volatilityData,
                               SurfaceInterpolatorType interpolatorType,
                               bool extraploate);

        virtual bool isOK();

        double getMonthsAheadOfAnchorDate(Date toDate) const;

        virtual bool isInRange(QuantLib::Date date) const;
        virtual bool isInRange(QuantLib::Date date, double strike, double forward) const;
        virtual bool extendsTo(Date date) const;

        virtual double getVolatility(Date toDate) const;
        virtual double getVolatility(Date toDate, double strike, double forward) const;
        // I need to over write these to deal with different day counters
        virtual double getStandardDeviation(Date toDate) const;
        virtual double getStandardDeviation(Date toDate, double strike, double forward) const;

        virtual void getVolAndSD(Date toDate, double strike, double forward, double &vol, double& sd) const;
        virtual void getVolAndSD(vector<Date> toDates, 
                                 double strike, 
                                 vector<double> forwards, 
                                 vector<double> &vols, 
                                 vector<double> &sds) const;

        virtual boost::shared_ptr<VolatilityRateSource> rollForward(Date toDate);
        virtual boost::shared_ptr<VolatilityRateSource> parallelBump(double spread);

        virtual void getRolledVariables(Date rolltoDate,
                                        vector<Date>& rolledObservationDates, 
                                        QuantLib::Matrix& rolledVols);

 
    protected:

        Date beginningOfAnchorDateMonth;
        boost::shared_ptr<DayCounter> simpleDayCounter;
    };
}

#endif