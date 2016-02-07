#ifndef FUTURESCURVEOBJECT_DEFINED
#define FUTURESCURVEOBJECT_DEFINED

#include <oh/libraryobject.hpp>
#include <sjd/ForwardCurve/FuturesCurve.h>

namespace sjdXLLObjects
{
    class CalendarMonthAverageFuturesCurveObject : public ObjectHandler::LibraryObject<sjd::CalendarMonthAverageFuturesCurve> {
      public:
        CalendarMonthAverageFuturesCurveObject(const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                                           const Date& anchorDate, vector<Date>& dates,
                                           vector<double>& futures, 
                                           bool allowExtrapolation, bool permanent);

        vector<double> getForward(const vector<QuantLib::Date>& dates);
    };
}

#endif