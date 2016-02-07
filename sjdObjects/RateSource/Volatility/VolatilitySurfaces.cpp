#include "VolatilitySurfaces.h"


namespace sjdObjects 
{
    /*======================================================================================
    sjdObjects::VolatilitySurfaceMoneyness

    =======================================================================================*/
    VolatilitySurfaceMoneyness::VolatilitySurfaceMoneyness(const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                                                                 Date anchorDate, 
                                                                 vector<Date> observationDates, 
                                                                 vector<double> moneyness,
                                                                 vector<vector<double>> volatilityData,
                                                                 SurfaceInterpolatorType interpolationType,
                                                                 bool permanent) 
            : DeterministicVolatilityRateSource(properties, permanent) 
    {
        boost::shared_ptr<sjd::ForwardRateSource> frs;
        libraryObject_ = boost::shared_ptr<sjd::VolatilitySurfaceMoneyness>(new
                            sjd::VolatilitySurfaceMoneyness(anchorDate, 
                                                            observationDates, 
                                                            moneyness, 
                                                            volatilityData, 
                                                            interpolationType, 
                                                            true));        
    }

    /*======================================================================================
    sjdObjects::VolatilitySurfaceMoneynessCMA

    =======================================================================================*/
    VolatilitySurfaceMoneynessCMA::VolatilitySurfaceMoneynessCMA(const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                                                                 Date anchorDate, 
                                                                 vector<Date> observationDates, 
                                                                 vector<double> moneyness,
                                                                 vector<vector<double>> volatilityData,
                                                                 SurfaceInterpolatorType interpolationType,
                                                                 bool permanent) 
            : DeterministicVolatilityRateSource(properties, permanent) 
    {
        boost::shared_ptr<sjd::ForwardRateSource> frs;
        libraryObject_ = boost::shared_ptr<sjd::VolatilitySurfaceMoneynessCMA>(new
                            sjd::VolatilitySurfaceMoneynessCMA(anchorDate, 
                                                               observationDates, 
                                                               moneyness, 
                                                               volatilityData, 
                                                               interpolationType, 
                                                               true));        
    }



    /*======================================================================================
    sjdObjects::VolatilitySurfaceDelta

    =======================================================================================*/
    VolatilitySurfaceDelta::VolatilitySurfaceDelta(const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                                                   Date anchorDate, 
                                                   vector<Date> observationDates, 
                                                   vector<double> delta,
                                                   vector<vector<double>> volatilityData,
                                                   SurfaceInterpolatorType interpolationType,
                                                   bool permanent) 
        : DeterministicVolatilityRateSource(properties, permanent) 
    {
        boost::shared_ptr<sjd::ForwardRateSource> frs;
        libraryObject_ = boost::shared_ptr<sjd::VolatilitySurfaceDelta>(new
                            sjd::VolatilitySurfaceDelta(anchorDate, 
                                                        observationDates, 
                                                        delta, 
                                                        volatilityData, 
                                                        interpolationType, 
                                                        true));        
    }
      
}