#include "DiscountCurveObject.h"

using namespace sjd;

namespace sjdXLLObjects 
{

    DiscountCurveObject::DiscountCurveObject(const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                                                 const Date& anchorDate, vector<Date>& dates,
                                                 vector<double>& discountFactors, const DayCounter& dayCounter,
                                                 bool allowExtrapolation, bool permanent)
                                                 : ObjectHandler::LibraryObject<DiscountCurve>(properties, permanent)
    {
        libraryObject_ = boost::shared_ptr<DiscountCurve>(new
                DiscountCurve(anchorDate, dates, discountFactors, dayCounter, allowExtrapolation));


    }

    vector<double> DiscountCurveObject::getDiscountFactor(const vector<QuantLib::Date>& dates)
    {
        return libraryObject_->getDiscountFactor(dates);
    }
}