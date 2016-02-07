#include "VolatilitySurfaceDelta.h"

namespace sjd 
{

    
    /*======================================================================================
    VolatilitySurfaceDelta 

    =======================================================================================*/
    VolatilitySurfaceDelta::VolatilitySurfaceDelta(Date anchorDateInput, 
                                                   vector<Date> observationDatesInput, 
                                                   vector<double> deltaInput,
                                                   vector<vector<double>> volatilityInput,
                                                   SurfaceInterpolatorType interpolatorTypeInput,
                                                   bool extrapolateInput)
    {
        errorTracking = boost::shared_ptr<sjdTools::ErrorTracking>(new sjdTools::ErrorTracking("VolatilitySurfaceDelta"));

        setInputs(anchorDateInput, 
                  observationDatesInput, 
                  deltaInput,
                  convertVectorOfVectorsToMatrix(volatilityInput),
                  interpolatorTypeInput,
                  extrapolateInput);
    }

    VolatilitySurfaceDelta::VolatilitySurfaceDelta(Date anchorDateInput, 
                                                   vector<Date> observationDatesInput, 
                                                   vector<double> deltaInput,
                                                   QuantLib::Matrix volatilityInput,
                                                   SurfaceInterpolatorType interpolatorTypeInput,
                                                   bool extrapolateInput)
    {
        errorTracking = boost::shared_ptr<sjdTools::ErrorTracking>(new sjdTools::ErrorTracking("VolatilitySurfaceDelta"));

        setInputs(anchorDateInput, 
                  observationDatesInput, 
                  deltaInput,
                  volatilityInput,
                  interpolatorTypeInput,
                  extrapolateInput);
    }

    bool VolatilitySurfaceDelta::isOK()
    {
        VolatiltiySurfaceRateSource::isOK();
        return !errorTracking->getHasErrors();
    }

    bool VolatilitySurfaceDelta::isInRange(QuantLib::Date date, double strike, double forward) const
    {
        if (allowExtrapolation)
        {
            return (date >= anchorDate);
        }
        double delta = calculateDeltaFromStrike(date, strike, forward);
        return interpolator->isInRange(dayCounter->yearFraction(anchorDate, date), delta);
    } 

    bool VolatilitySurfaceDelta::isInRange(QuantLib::Date date) const
    {
        if (allowExtrapolation)
        {
            return true;
        }
        return interpolator->isInRange(dayCounter->yearFraction(anchorDate, date), 0.5);
    } 

    double VolatilitySurfaceDelta::getVolatility(Date toDate) const
    {
      return getVolatility(toDate, 1.0, 1.0);
    }

    double VolatilitySurfaceDelta::getVolatility(Date toDate, double strike, double forward) const
    {
      double delta = calculateDeltaFromStrike(toDate, strike, forward);
      double yf = dayCounter->yearFraction(anchorDate, toDate);
      return (*interpolator)(yf, delta, allowExtrapolation);
    }

   double VolatilitySurfaceDelta::getStandardDeviation(Date toDate) const
    {
      return getStandardDeviation(toDate, 1.0, 1.0);
    };

    double VolatilitySurfaceDelta::getStandardDeviation(Date toDate, double strike, double forward) const
    {
        double vol = getVolatility(toDate, strike, forward);
        double yf = dayCounter->yearFraction(anchorDate, toDate);
        return vol * sqrt(yf);
    }

    double VolatilitySurfaceDelta::calculateDeltaFromStrike(Date date, double strike, double forward) const
    {
      double yf = dayCounter->yearFraction(anchorDate, date);
        double accuracy = 1.0e-8;
        size_t maxItterates = 20;
        double sqrtttm = sqrt(yf); 

        double guess1 = 50, guess2 = 50;
        double vol1, sd1 = 0.2, diff = accuracy + 1;
       
        Black76Put put(forward, strike, sd1, 1);
        
        size_t counter = 0;

        while ((counter < maxItterates) && (diff > accuracy)) 
        {
            guess1 = guess2;
            try 
            {
                vol1 = (*interpolator)(yf, guess1, true); // don't want to fall over here        
            }
            catch (std::exception)
            {
                return 0;
            }
            sd1 = vol1 * sqrtttm;
            put.setStandardDeviation(sd1);
            guess2 = -put.getDelta() * 100;        
            diff = abs(guess1 - guess2);
            ++counter;
        }        
        return guess2;
    }

    double VolatilitySurfaceDelta::calculateStrikeFromDelta(Date date, double delta, double forward) const
    {        
        QuantLib::Real accuracy = 1.0e-12;
        QuantLib::Bisection solver;  
        double yf = dayCounter->yearFraction(anchorDate, date);
        double vol = (*interpolator)(yf, delta, allowExtrapolation);
        double sd = vol * sqrt(yf);
        double strike;

        double guess = forward; //frs->getForward(date);
        double lowerBound = 0.1 * guess;
        double upperBound = 2.0 * guess;
        try 
        {
            strike = solver.solve(BlackPutImpliedStrike(delta, forward,sd),
                                  accuracy,
                                  guess,
                                  lowerBound,
                                  upperBound);        
        } 
        catch (...) 
        {
            strike = 0;
        }
        return strike;      
    }

    boost::shared_ptr<VolatilityRateSource> VolatilitySurfaceDelta::parallelBump(double spread)
    {
        vector<Date> newDates;
        QuantLib::Matrix volatilitySubset;
      
        getRolledVariables(anchorDate, newDates, volatilitySubset);
        Matrix spreadMatrix = Matrix(volatilitySubset.rows(), volatilitySubset.columns(), spread);
        Matrix bumpedVolMatrix = volatilitySubset + spreadMatrix;

        return boost::shared_ptr<VolatilityRateSource>(
            new VolatilitySurfaceDelta(anchorDate, 
                                       newDates, 
                                       strikeDimension,
                                       volatilitySubset,
                                       interpolatorType,
                                       allowExtrapolation));
    }

    boost::shared_ptr<VolatilityRateSource> VolatilitySurfaceDelta::rollForward(Date toDate)
    {
        vector<Date> newDates;
        QuantLib::Matrix volatilitySubset;
      
        getRolledVariables(toDate, newDates, volatilitySubset);

        return boost::shared_ptr<VolatilityRateSource>(
            new VolatilitySurfaceDelta(toDate, 
                                       newDates, 
                                       strikeDimension,
                                       volatilitySubset,
                                       interpolatorType,
                                       allowExtrapolation));
    }


    /*======================================================================================
    SimpleDeltaSurface

    =======================================================================================*/
    SimpleDeltaSurface::SimpleDeltaSurface(vector<double> timesInput, 
                                           vector<double> deltaInput, 
                                           vector<vector<double>> volatility, 
                                           bool extrapolate, 
                                           string interpolationType)
        : extrapolate(extrapolate), times(timesInput), delta(deltaInput)
    {
        className = "SimpleDeltaSurface";
        boost::algorithm::to_lower(interpolationType);
        boost::algorithm::trim(interpolationType);
        if (!((interpolationType.compare("bilinear") == 0) || (interpolationType.compare("bicubic") == 0))) 
        {
            isOKVariable = false;
            errorMessage = className + "Interpolation Type must be either Bilinear or Bicubic";
            return;
        }
        isOKVariable = true;
        errorMessage = "";
        if ((times.size() == 0) || (delta.size() == 0) || (volatility.size() == 0)) 
        {
            isOKVariable = false;
            errorMessage = className + "One of the input vectors is empty";
            return;
        }
        size_t rows = volatility.size();
        size_t columns = volatility[0].size();
        for (size_t i = 0; i < rows; ++i) 
        {
            if (volatility[i].size() != columns) 
            {
                isOKVariable = false;
                errorMessage = className + "The input volatlity is not square";
                return;
            }
        }
        if (!isStrictlyIncreasing<double>(times)) 
        {
            isOKVariable = false;
            errorMessage = className + "Times are not strictly increasing";            
            return;
        }
        if (!isStrictlyIncreasing<double>(delta)) 
        {
            isOKVariable = false;
            errorMessage = "Deltas are not strictly increasing";            
            return;
        }
        if (delta[0] < 1.0) 
        {
            for (size_t i = 0; i < delta.size(); ++i)
            {
                delta[i] *= 100;
            }
        }
        if (times[0] > 0) 
        {
            times.insert(times.begin(), 0);
            for (size_t i = 0; i < volatility.size(); ++i) 
            {
                double vol = volatility[i][0];
                volatility[i].insert(volatility[i].begin(), vol);
            }
        }
        volatilityMatrix = Matrix(rows, columns);
        for (size_t i = 0; i < rows; ++i) 
        {
            for (size_t j = 0; j < columns; ++j) 
            {
                volatilityMatrix[i][j] = volatility[i][j];
            }
        }
        if (interpolationType.compare("bilinear") == 0)
        {
            interpolator = boost::shared_ptr<Interpolation2D>(
                new BilinearInterpolation(times.begin(), 
                                          times.end(), 
                                          delta.begin(), 
                                          delta.end(), 
                                          volatilityMatrix));
        }
        else 
        {
            interpolator = boost::shared_ptr<Interpolation2D>(
                new BicubicSpline(times.begin(), 
                                  times.end(), 
                                  delta.begin(), 
                                  delta.end(), 
                                  volatilityMatrix));
        }
    }

    bool SimpleDeltaSurface::isOK() 
    {
        return isOKVariable;
    }

    string SimpleDeltaSurface::getErrorMessage() 
    {
        return errorMessage;
    }

    bool SimpleDeltaSurface::isInDeltaRange(double time, double delta)
    {
        if ((extrapolate) || interpolator->isInRange(time, delta))
        {
            return true;
        }
        return false;
    }

    bool SimpleDeltaSurface::isInMoneynessRange(double time, double moneyness)
    {
        if (extrapolate)
        {
            return true;
        }
        double fwd = 1;
        double strike = moneyness + fwd;
        double vol = calculateDeltaFromStrike(fwd, strike, time);
        if (abs(vol)<1e-8)
        {
            return false;
        }
        return true;
    }

    double SimpleDeltaSurface::getVolatility(double time) const
    {
        double delta = calculateDeltaFromStrike(1, 1, time);
        return getVolatilityForDelta(time, delta);
    }

    double SimpleDeltaSurface::getVolatilityForDelta(double time, double delta) const
    {
        if ((extrapolate) || (interpolator->isInRange(time, delta)))
        {
            return (*interpolator)(time, delta, extrapolate);
        }
        return numeric_limits<double>::quiet_NaN();
    }

    double SimpleDeltaSurface::getVolatilityForMoneyness(double time, double moneyness) const
    {
        if (time <=0)
        {
            return 0;
        }
        double fwd = 1;
        double strike = moneyness + fwd;
        double delta = calculateDeltaFromStrike(fwd, strike, time);
        return getVolatilityForDelta(time, delta);
    }

    // will return 0 if outside the interpolation range
    double SimpleDeltaSurface::calculateDeltaFromStrike(double forward, double strike, double time) const
    {
        double accuracy = 1.0e-8;
        size_t maxItterates = 20;
        double sqrtttm = sqrt(time); 
        double guess1 = 50, guess2 = 50;
        double vol1 = 0, sd1 = 0.2, diff = accuracy + 1;       
        Black76Put put(forward, strike, sd1, 1);        
        size_t counter = 0;
        while ((counter < maxItterates) && (diff > accuracy)) 
        {
            guess1 = guess2;
            if (interpolator->isInRange(time, guess1))
            {
                vol1 = (*interpolator)(time, guess1, extrapolate);  
            }
            sd1 = vol1 * sqrtttm;
            put.setStandardDeviation(sd1);
            guess2 = -put.getDelta() * 100;        
            diff = abs(guess1 - guess2);
            ++counter;
        }        
        return guess2;
    }
}