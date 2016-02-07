#include "VolatilitySurfaceMoneyness.h"

namespace sjd 
{

   /*======================================================================================
   VolatilitySurfaceMoneyness 

    =======================================================================================*/
   VolatilitySurfaceMoneyness::VolatilitySurfaceMoneyness(Date anchorDateInput, 
                                       vector<Date> observationDatesInput, 
                                       vector<double> moneynessInput,
                                       vector<vector<double>> volatilityDataInput,
                                       SurfaceInterpolatorType interpolatorTypeInput,
                                       bool extrapolateInput) 
    {
        errorTracking = boost::shared_ptr<sjdTools::ErrorTracking>(new sjdTools::ErrorTracking("VolatilitySurfaceMoneyness"));

        setInputs(anchorDateInput, 
                  observationDatesInput, 
                  moneynessInput,
                  convertVectorOfVectorsToMatrix(volatilityDataInput),
                  interpolatorTypeInput,
                  extrapolateInput);
    }

    VolatilitySurfaceMoneyness::VolatilitySurfaceMoneyness(Date anchorDateInput, 
                                       vector<Date> observationDatesInput, 
                                       vector<double> moneynessInput,
                                       QuantLib::Matrix volatilityDataInput,
                                       SurfaceInterpolatorType interpolatorTypeInput,
                                       bool extrapolateInput) 
    {
        errorTracking = boost::shared_ptr<sjdTools::ErrorTracking>(new sjdTools::ErrorTracking("VolatilitySurfaceMoneyness"));

        setInputs(anchorDateInput, 
                  observationDatesInput, 
                  moneynessInput,
                  volatilityDataInput,
                  interpolatorTypeInput,
                  extrapolateInput);
    }

    bool VolatilitySurfaceMoneyness::isOK()
    {
        VolatiltiySurfaceRateSource::isOK();
        return !errorTracking->getHasErrors();
    }

   bool VolatilitySurfaceMoneyness::isInRange(QuantLib::Date date) const 
    {
        if (allowExtrapolation)
        {
            return true;
        }
        return interpolator->isInRange(dayCounter->yearFraction(anchorDate, date), 0.0);
    }

   bool VolatilitySurfaceMoneyness::isInRange(QuantLib::Date date, double strike, double forward) const 
    {
        if (allowExtrapolation)
        {
            return true;
        }
        double moneyness = (strike - forward) / forward;
        return interpolator->isInRange(dayCounter->yearFraction(anchorDate, date), moneyness);
    }

    double VolatilitySurfaceMoneyness::getVolatility(Date toDate) const                               
    {
        if (!isInRange(toDate)) 
        {
            return numeric_limits<double>::quiet_NaN();
        }
        double yf = dayCounter->yearFraction(anchorDate, toDate);
        return (*interpolator)(yf, 0.0, allowExtrapolation);

   }

    double VolatilitySurfaceMoneyness::getVolatility(Date toDate, double strike, double forward) const 
    {
        if (!isInRange(toDate, strike, forward)) 
        {
            return numeric_limits<double>::quiet_NaN();
        }
        double yf = dayCounter->yearFraction(anchorDate, toDate);
        double moneyness = (strike - forward) / forward;
        return (*interpolator)(yf, moneyness, allowExtrapolation);
    }

    boost::shared_ptr<VolatilityRateSource> VolatilitySurfaceMoneyness::parallelBump(double spread)
    {
        vector<Date> newDates;
        QuantLib::Matrix volatilitySubset;

        getRolledVariables(anchorDate, newDates, volatilitySubset);
        Matrix spreadMatrix = Matrix(volatilitySubset.rows(), volatilitySubset.columns(), spread);
        Matrix bumpedVolMatrix = volatilitySubset + spreadMatrix;

        return boost::shared_ptr<VolatilityRateSource>(new VolatilitySurfaceMoneyness(anchorDate, 
                                                                             newDates, 
                                                                             strikeDimension,
                                                                             bumpedVolMatrix,
                                                                             interpolatorType,
                                                                             allowExtrapolation));
    }

    boost::shared_ptr<VolatilityRateSource> VolatilitySurfaceMoneyness::rollForward(Date toDate)
    {
        vector<Date> newDates;
        QuantLib::Matrix volatilitySubset;

        getRolledVariables(toDate, newDates, volatilitySubset);

        return boost::shared_ptr<VolatilityRateSource>(new VolatilitySurfaceMoneyness(toDate, 
                                                                   newDates, 
                                                                   strikeDimension,
                                                                   volatilitySubset,
                                                                   interpolatorType,
                                                                   allowExtrapolation));
    }
}