#include "VolatilityCurve.h"


namespace sjdObjects 
{
    /*======================================================================================
    sjdObjects::VolatilityCurve

    =======================================================================================*/
    VolatilityCurve::VolatilityCurve(const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                        Date anchorDate, 
                        vector<Date> observationDates, 
                        vector<double> volatility, 
                        DayCounter dayCounter,
                        sjd::ArrayInterpolatorType type,
                        bool allowExtrapolation,
                        bool permanent) : DeterministicVolatilityRateSource(properties, permanent) 
    {
        boost::shared_ptr<DayCounter> dayCounterPtr = boost::shared_ptr<DayCounter>(new DayCounter(dayCounter));

        libraryObject_ = boost::shared_ptr<sjd::VolatilityCurve>(new
                                sjd::VolatilityCurve(anchorDate, 
                                                     observationDates, 
                                                     volatility, 
                                                     dayCounterPtr, 
                                                     type, 
                                                     allowExtrapolation));
    }

    /*======================================================================================
    sjdObjects::VolatilityCurveCMA

    =======================================================================================*/
    VolatilityCurveCMA::VolatilityCurveCMA(const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                                           Date anchorDate, 
                                           vector<Date> observationDates, 
                                           vector<double> volatility, 
                                           DayCounter dayCounter,
                                           sjd::ArrayInterpolatorType type,
                                           bool allowExtrapolation,
                                           bool permanent) : DeterministicVolatilityRateSource(properties, permanent) 
    {
        boost::shared_ptr<DayCounter> dayCounterPtr = boost::shared_ptr<DayCounter>(new DayCounter(dayCounter));

        libraryObject_ = boost::shared_ptr<sjd::VolatilityCurveCMA>(new
                                sjd::VolatilityCurveCMA(anchorDate, 
                                                        observationDates, 
                                                        volatility, 
                                                        dayCounterPtr, 
                                                        type, 
                                                        allowExtrapolation));
    }
}