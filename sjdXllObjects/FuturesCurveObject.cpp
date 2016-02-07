#include "FuturesCurveObject.h"

using namespace sjd;

namespace sjdXLLObjects 
{

    CalendarMonthAverageFuturesCurveObject::CalendarMonthAverageFuturesCurveObject(const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                                                 const Date& anchorDate, vector<Date>& dates,
                                                 vector<double>& futures, 
                                                 bool allowExtrapolation, bool permanent)
                                                 : ObjectHandler::LibraryObject<CalendarMonthAverageFuturesCurve>(properties, permanent)
    {
        libraryObject_ = boost::shared_ptr<CalendarMonthAverageFuturesCurve>(new
                CalendarMonthAverageFuturesCurve(anchorDate, dates, futures, allowExtrapolation));


    }

    vector<double> CalendarMonthAverageFuturesCurveObject::getForward(const vector<QuantLib::Date>& dates)
    {
        return libraryObject_->getForward(dates);
    }

}