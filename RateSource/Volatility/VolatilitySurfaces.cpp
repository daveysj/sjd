#include "VolatilitySurfaces.h"

namespace sjd 
{

    /*======================================================================================
   VolatiltiySurfaceRateSource

    =======================================================================================*/
    bool VolatiltiySurfaceRateSource::isOK()
    {
        DeterministicVolatilityRateSource::isOK();

        if ((observationDates.size() == 0) || (strikeDimension.size() == 0) || (volatility.empty())) 
        {
            errorTracking->populateErrorMessage("One of observation dates, moneyness or volatility is empty");
        }
        if ((observationDates.size() != volatility.columns()) || (strikeDimension.size() != volatility.rows()) ) 
        {
            errorTracking->populateErrorMessage("Input vectors and matricies have inconsistent dimension");
        }
        if (!isStrictlyIncreasing<Date>(observationDates))
        {
            errorTracking->populateErrorMessage("Input dates not strictly increasing");
        }
        if (!isStrictlyIncreasing<double>(time)) 
        {
            errorTracking->populateErrorMessage("Input times not strictly increasing");
        }
        if (!isStrictlyIncreasing<double>(strikeDimension))
        {
            errorTracking->populateErrorMessage("Strike dimension inputs are not strictly increasing");
        }
        if ((interpolatorType!=BILINEAR) && (interpolatorType!=BICUBIC)) 
        {
            errorTracking->populateErrorMessage("Interpolator Type must be either Bilinear or Bicubic");
        }
        if (interpolator == NULL) 
        {
            errorTracking->populateErrorMessage("Interpolator not set");
        }
        if (finalDate < anchorDate) 
        {
            errorTracking->populateErrorMessage("Final date before anchor date");
        }
        return !errorTracking->getHasErrors();
    }

    vector<Date> VolatiltiySurfaceRateSource::getObservationDates()
    {
        return observationDates;
    }

    void VolatiltiySurfaceRateSource::setInputs(Date anchorDateInput, 
                                                vector<Date> observationDatesInput, 
                                                vector<double> strikeDimensionInput,
                                                QuantLib::Matrix volatilityInput,
                                                SurfaceInterpolatorType interpolatorTypeInput,
                                                bool extrapolateInput)
    {
        if (observationDatesInput.size() == 0 ||
             strikeDimensionInput.size() == 0 ||
             volatilityInput.rows() * volatilityInput.columns() == 0)
        {
            observationDates = vector<Date>();
            strikeDimension = vector<double>();
            volatility = Matrix(0,0);
            return;
        }
        anchorDate = anchorDateInput;
        dayCounter = boost::shared_ptr<DayCounter>(new Actual365Fixed());
        allowExtrapolation = extrapolateInput;

        strikeDimension = strikeDimensionInput;
        observationDates = observationDatesInput;
        if (observationDates[0] > anchorDate) 
        {
            observationDates.insert(observationDates.begin(), anchorDate);
            volatility = QuantLib::Matrix(volatilityInput.rows(), volatilityInput.columns()+1);
            for (size_t i = 0; i < volatilityInput.rows(); ++i) 
            {
                volatility[i][0] = volatilityInput[i][0];
                for (size_t j = 0; j < volatilityInput.columns(); ++j) 
                {
                    volatility[i][j+1] = volatilityInput[i][j];
                }
            }
        }
        else 
        {
            volatility = volatilityInput;
        }
        
        time.clear();
        for (size_t i = 0; i < observationDates.size(); ++i) 
        {
            double yf = dayCounter->yearFraction(anchorDate, observationDates[i]);
            time.push_back(yf);
        }

        if (observationDates.size() > 0) 
        {
            finalDate = observationDates.back();
        }
        else 
        {
         finalDate = Date(01,Jan,1901);
        }
        setInterpolatorType(interpolatorTypeInput);
    }

    SurfaceInterpolatorType VolatiltiySurfaceRateSource::getInterpolatorType()
    {
        return interpolatorType;
    }

    void VolatiltiySurfaceRateSource::setInterpolatorType(SurfaceInterpolatorType interpolatorTypeInput)
    {
        if ((time.size() == 0) || (strikeDimension.size() == 0) || (volatility.rows() * volatility.columns() == 0)) 
        {
            return;
        }
        interpolatorType = interpolatorTypeInput;        
        if (interpolatorType == BILINEAR) 
        {
            interpolator = boost::shared_ptr<Interpolation2D>(new QuantLib::BilinearInterpolation(time.begin(), 
                                                                                                  time.end(), 
                                                                                                  strikeDimension.begin(), 
                                                                                                  strikeDimension.end(), 
                                                                                                  volatility));        
        }
        else if (interpolatorType = BICUBIC) 
        {
            interpolator = boost::shared_ptr<Interpolation2D>(new QuantLib::BicubicSpline(time.begin(), 
                                                                                          time.end(), 
                                                                                          strikeDimension.begin(), 
                                                                                          strikeDimension.end(), 
                                                                                          volatility));
        }
        else 
        {            
            interpolator = boost::shared_ptr<Interpolation2D>(new QuantLib::BicubicSpline(time.begin(), 
                                                                                          time.end(), 
                                                                                          strikeDimension.begin(), 
                                                                                          strikeDimension.end(), 
                                                                                          volatility));
            interpolatorType = BICUBIC;
        }
    }

   void VolatiltiySurfaceRateSource::getRolledVariables(Date toDate,
                                                        vector<Date>& newDates, 
                                                        QuantLib::Matrix& volatilitySubset)
   {
        if (!isInRange(toDate))
        {
            newDates.clear();
            newDates.push_back(toDate);
            volatilitySubset = QuantLib::Matrix(0, 0);
            return;
        }
        else 
        {
            size_t offset = 0;
            for (size_t i = 0; i < observationDates.size(); ++i)
            {
                double time = dayCounter->yearFraction(toDate, observationDates[i]);
                if (time >= 0) 
                {
                    newDates.push_back(observationDates[i]);
                }
                else 
                {
                    ++offset;
                }
            }
            volatilitySubset = QuantLib::Matrix(strikeDimension.size(), newDates.size());
            for (size_t i = 0; i < strikeDimension.size(); ++i) 
            {
                for (size_t j = 0; j < newDates.size(); ++j)
                {
                    volatilitySubset[i][j] = volatility[i][j+offset];
                }
            }
            return;
        }
   }

    QuantLib::Matrix VolatiltiySurfaceRateSource::convertVectorOfVectorsToMatrix(vector<vector<double>> volatilityData)
    {
        if (volatilityData.size() == 0) 
        {
            return Matrix(1, 1, 0);
        }
        else 
        {
            size_t rows, columns;
            rows = volatilityData.size();
            columns = volatilityData[0].size();
            Matrix M = Matrix(rows, columns);
            for (size_t i = 0; i < rows; ++i) 
            {
                if (columns != volatilityData[i].size())
                {
                    return Matrix(1, 1, 0);;
                }
                for (size_t j = 0; j < columns; ++j) 
                {
                    M[i][j] = volatilityData[i][j];
                }
            }
            return M;
        }
    }
}