#ifndef DISCOUNTCURVEOBJECT_DEFINED
#define DISCOUNTCURVEOBJECT_DEFINED

#include <oh/libraryobject.hpp>
#include <sjd/DiscountCurves/DiscountCurves.h>

namespace sjdXLLObjects
{
    class DiscountCurveObject : public ObjectHandler::LibraryObject<sjd::DiscountCurve> {
      public:
        DiscountCurveObject(const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                              const Date& anchorDate, vector<Date>& dates,
                              vector<double>& discountFactors, const DayCounter& dayCounter,
                              bool allowExtrapolation, bool permanent);

        vector<double> getDiscountFactor(const vector<QuantLib::Date>& dates);
    };
}

#endif