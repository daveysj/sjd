#include "FixingCurves.h"


namespace sjdObjects {
   /*======================================================================================
   DailyFixingCurve 

    =======================================================================================*/
    DailyFixingCurve::DailyFixingCurve(const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                         vector<Date> fixingDates, 
                         vector<double> fixingRates,
                        bool permanent) : FixingRateSource(properties, permanent) 
    {
        libraryObject_ = boost::shared_ptr<sjd::RollingFixingRateSource>(new sjd::DailyFixingCurve(fixingDates, fixingRates));
    }


   /*======================================================================================
   ForwardCurveCMAStochasticFwdVol 

    =======================================================================================*/
    MonthlyFixingCurve::MonthlyFixingCurve(const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                                         vector<Date> fixingDates, 
                                         vector<double> fixingRates,
                                       bool permanent) : FixingRateSource(properties, permanent) 
    {
        libraryObject_ = boost::shared_ptr<sjd::RollingFixingRateSource>(new
                            sjd::MonthlyFixingCurve(fixingDates, fixingRates));
    }

}