#ifndef SJD_OBJ_VOLATILITYSURFACES
#define SJD_OBJ_VOLATILITYSURFACES

#include <sjd\RateSource\Volatility\VolatilitySurfaces.h>
#include <sjd\RateSource\Volatility\VolatilitySurfaceMoneyness.h>
#include <sjd\RateSource\Volatility\VolatilitySurfaceMoneynessCMA.h>
#include <sjd\RateSource\Volatility\VolatilitySurfaceDelta.h>

#include <sjd\sjdObjects\RateSource\Volatility\VolatilityRateSource.h>

#include <ql\time\date.hpp>
#include <ql\time\daycounters\simpledaycounter.hpp>

using namespace std;
using namespace QuantLib;

namespace sjd 
{
    enum SurfaceInterpolatorType;
}

namespace sjdObjects 
{
    /*======================================================================================
    sjdObjects::VolatilitySurfaceMoneyness

    =======================================================================================*/
    class VolatilitySurfaceMoneyness : public DeterministicVolatilityRateSource
    {
    public:
        VolatilitySurfaceMoneyness(const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                                                    Date anchorDate, 
                                                    vector<Date> observationDates, 
                                                    vector<double> moneyness,
                                                    vector<vector<double>> volatilityData,
                                                    SurfaceInterpolatorType interpolationType,
                                                    bool permanent);      

    protected:
        OH_OBJ_CTOR(VolatilitySurfaceMoneyness, DeterministicVolatilityRateSource);
    };


    /*======================================================================================
    sjdObjects::VolatilitySurfaceMoneynessCMA

    =======================================================================================*/
    class VolatilitySurfaceMoneynessCMA : public DeterministicVolatilityRateSource
    {
    public:
        VolatilitySurfaceMoneynessCMA(const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                                                    Date anchorDate, 
                                                    vector<Date> observationDates, 
                                                    vector<double> moneyness,
                                                    vector<vector<double>> volatilityData,
                                                    SurfaceInterpolatorType interpolationType,
                                                    bool permanent);      

    protected:
        OH_OBJ_CTOR(VolatilitySurfaceMoneynessCMA, DeterministicVolatilityRateSource);
    };


    /*======================================================================================
    VolatilitySurfaceDelta 

    =======================================================================================*/
    class VolatilitySurfaceDelta : public DeterministicVolatilityRateSource
    {
    public:
        VolatilitySurfaceDelta(const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                                                    Date anchorDate, 
                                                    vector<Date> observationDates, 
                                                    vector<double> delta,
                                                    vector<vector<double>> volatilityData,
                                                    SurfaceInterpolatorType interpolationType,
                                                    bool permanent);             

    protected:
        OH_OBJ_CTOR(VolatilitySurfaceDelta, DeterministicVolatilityRateSource);
    };

}

#endif
