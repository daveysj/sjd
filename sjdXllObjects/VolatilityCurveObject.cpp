#include "VolatilityCurveObject.h"

using namespace sjd;

namespace sjdXLLObjects 
{

    VolatilityCurveObject::VolatilityCurveObject(const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                                                 const Date& anchorDate, vector<Date>& dates,
                                                 vector<double>& volatility, const DayCounter& dayCounter,
                                                 VolatilityInterpolatorType type,
                                                 bool allowExtrapolation, bool permanent)
                                                 : ObjectHandler::LibraryObject<VolatilityCurve>(properties, permanent)
    {
        libraryObject_ = boost::shared_ptr<VolatilityCurve>(new
                VolatilityCurve(anchorDate, dates, volatility, 
                                dayCounter, type, allowExtrapolation));


    }

    vector<double> VolatilityCurveObject::getVolatilty(const vector<QuantLib::Date>& dates)
    {
        return libraryObject_->getVolatility(dates);
    }

    vector<double> VolatilityCurveObject::getStandardDeviation(const vector<QuantLib::Date>& dates)
    {
        return libraryObject_->getStandardDeviation(dates);
    }


}