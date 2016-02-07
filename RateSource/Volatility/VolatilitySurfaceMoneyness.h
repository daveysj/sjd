
#ifndef SJD_EUROPEANMONEYNESS_INCLUDED
#define SJD_EUROPEANMONEYNESS_INCLUDED

#include "VolatilitySurfaces.h"


using namespace QuantLib;


namespace sjd 
{

    /*======================================================================================
   VolatilitySurfaceMoneyness

    European Moneyness Surface
    =======================================================================================*/
    class VolatilitySurfaceMoneyness : public VolatiltiySurfaceRateSource
        // VolSurfaceMoneyness
    {
    public:
        
      VolatilitySurfaceMoneyness(Date anchorDate, 
                         vector<Date> observationDates, 
                         vector<double> moneyness,
                         QuantLib::Matrix volatilityData,
                         SurfaceInterpolatorType interpolatorType,
                         bool extrapolate = true); // row = moneyness, column = time
        
      VolatilitySurfaceMoneyness(Date anchorDate, 
                         vector<Date> observationDates, 
                         vector<double> moneyness,
                         vector<vector<double>> volatilityData,
                         SurfaceInterpolatorType interpolatorType,
                         bool extrapolate = true); // row = moneyness, column = time

        virtual ~VolatilitySurfaceMoneyness() {};

        virtual bool isOK();

        virtual bool isInRange(QuantLib::Date date) const;
        virtual bool isInRange(QuantLib::Date date, double strike, double forward) const;

        // Returns vol for a moneyness of 0
        virtual double getVolatility(Date toDate) const;
        virtual double getVolatility(Date toDate, double strike, double forward) const;

        virtual boost::shared_ptr<VolatilityRateSource> parallelBump(double spread);
        virtual boost::shared_ptr<VolatilityRateSource> rollForward(Date toDate);

    };
}

#endif