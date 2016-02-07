
#ifndef SJD_DELTA_INCLUDED
#define SJD_DELTA_INCLUDED

#include "VolatilitySurfaces.h"


using namespace QuantLib;


namespace sjd 
{

    /*======================================================================================
    BlackPutImpliedStrike 

    A classes built to utilise the QuantLib "solver framework" to imply the standard 
    deviation an option given all the other inputs. These all now solve for Standard 
    Deviation rather than Volatility. It is up to the user to do the simple conversion
    =======================================================================================*/
    class BlackPutImpliedStrike 
    {
    public:
        BlackPutImpliedStrike(double delta, double forward, double standardDeviation) :     
            delta(delta/100), forward(forward), standardDeviation(standardDeviation) 
        {
            put = boost::shared_ptr<Black76Option>(new Black76Put(forward, forward, standardDeviation, 1.0));
        };
        
        double operator()(double strikeEstimate) const 
        {
            put->setStrike(strikeEstimate);
            double d = put->getDelta();
            return delta + put->getDelta(); // put delta is negative
        }
    
    private:
        double delta, forward, standardDeviation;
        boost::shared_ptr<Black76Option> put;
    };


    /*======================================================================================
    VolatilitySurfaceDelta 

    =======================================================================================*/
    class VolatilitySurfaceDelta : public VolatiltiySurfaceRateSource
    {
    public:
        // Note that delta should be for a put and therefore must be a strictly increasing vector.
        // Also use 50 NOT 0.5 for 50 delta options
        VolatilitySurfaceDelta(Date anchorDate, 
                               vector<Date> observationDates, 
                               vector<double> deltas,
                               vector<vector<double>> volatilityData,
                               SurfaceInterpolatorType interpolatorType,
                               bool extrapolate = false); // row = delta, column = time

        VolatilitySurfaceDelta(Date anchorDate, 
                               vector<Date> observationDates, 
                               vector<double> deltas,
                               QuantLib::Matrix volatilityData,
                               SurfaceInterpolatorType interpolatorType,
                               bool extrapolate = false); // row = delta, column = time

        virtual ~VolatilitySurfaceDelta() {};

        virtual bool isOK();

        virtual bool isInRange(QuantLib::Date date) const;
        virtual bool isInRange(QuantLib::Date date, double strike, double forward) const;

        virtual double getVolatility(Date toDate) const;
        virtual double getVolatility(Date toDate, double strike, double forward) const;

        virtual double getStandardDeviation(Date toDate) const; 
        virtual double getStandardDeviation(Date toDate, double strike, double forward) const;

        double calculateDeltaFromStrike(Date date, double strike, double forward) const;
        double calculateStrikeFromDelta(Date date, double delta, double forward) const;
      
        virtual boost::shared_ptr<VolatilityRateSource> parallelBump(double spread);
        virtual boost::shared_ptr<VolatilityRateSource> rollForward(Date toDate);
    };
   

    /*======================================================================================
    SimpleDeltaSurface

    Used for excel *formula* rather than excel *objects*
    =======================================================================================*/
    class SimpleDeltaSurface
    {
    public:
        SimpleDeltaSurface(vector<double> times, 
                           vector<double> delta, 
                           vector<vector<double>> volatility, 
                           bool extrapolate, 
                           string interpolationType);
        bool isOK();
        string getErrorMessage();

        bool isInDeltaRange(double time, double delta);
        bool isInMoneynessRange(double time, double moneyness);

        double getVolatility(double time) const;
        double getVolatilityForDelta(double time, double delta) const;
        double getVolatilityForMoneyness(double time, double moneyness) const;

    private:
        double calculateDeltaFromStrike(double forward, double strike, double time) const;

        bool isOKVariable, extrapolate;
        vector<double> times, delta;
        QuantLib::Matrix volatilityMatrix;
        boost::shared_ptr<Interpolation2D> interpolator;
        string errorMessage, interpolationType, className;
    };
}

#endif