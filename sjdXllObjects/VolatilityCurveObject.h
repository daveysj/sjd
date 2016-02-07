#ifndef VOLATILITYCURVEOBJECT_DEFINED
#define VOLATILITYCURVEOBJECT_DEFINED

#include <oh/libraryobject.hpp>
#include <sjd/Volatility/VolatilityCurve.h>

namespace sjdXLLObjects
{
    class VolatilityCurveObject : public ObjectHandler::LibraryObject<sjd::VolatilityCurve> {
      public:
        VolatilityCurveObject(const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                              const Date& anchorDate, vector<Date>& dates,
                              vector<double>& volatility, const DayCounter& dayCounter,
                              sjd::VolatilityInterpolatorType type,
                              bool allowExtrapolation, bool permanent);

        vector<double> getVolatilty(const vector<QuantLib::Date>& dates);

        vector<double> getStandardDeviation(const vector<QuantLib::Date>& dates);
    };

}

#endif