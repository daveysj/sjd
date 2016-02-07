#ifndef SJD_FUTURESCURVEVALUEOBJECT
#define SJD_FUTURESCURVEVALUEOBJECT

#include <oh/valueobject.hpp>
#include <string>
#include <vector>
#include <set>
#include <boost/serialization/map.hpp>
#include <boost/algorithm/string/case_conv.hpp>

namespace sjdXLLObjects { namespace ValueObjects {

    class CalendarMonthAverageFuturesCurveValueObject : public ObjectHandler::ValueObject {
        friend class boost::serialization::access;
    public:
        CalendarMonthAverageFuturesCurveValueObject() {}
        CalendarMonthAverageFuturesCurveValueObject(
            const std::string& objectId,
            const ObjectHandler::property_t& anchorDate,
            const std::vector<ObjectHandler::property_t>& curveDates,
            const std::vector<double>& futures,
            const bool& allowExtrapoation,
            bool permanent);

        const std::set<std::string>& getSystemPropertyNames() const;
        std::vector<std::string> getPropertyNamesVector() const;
        ObjectHandler::property_t getSystemProperty(const std::string&) const;
        void setSystemProperty(const std::string& name, const ObjectHandler::property_t& value);

    protected:
        static const char* mPropertyNames[];
        static std::set<std::string> mSystemPropertyNames;

        ObjectHandler::property_t anchorDate_;
        std::vector<ObjectHandler::property_t> curveDates_;
        std::vector<double> futures_;
        bool allowExtrapolation_;
        bool permanent_;


        template<class Archive>
        void serialize(Archive& ar, const unsigned int) {
        boost::serialization::void_cast_register<qlDiscountCurve, ObjectHandler::ValueObject>(this, this);
            ar  & boost::serialization::make_nvp("ObjectId", objectId_)
                & boost::serialization::make_nvp("ClassName", className_)
                & boost::serialization::make_nvp("AnchorDate", anchorDate_)
                & boost::serialization::make_nvp("CurveDates", curveDates_)
                & boost::serialization::make_nvp("Futures", futures_)
                & boost::serialization::make_nvp("Extrapolates", allowExtrapolation_)
                & boost::serialization::make_nvp("Permanent", Permanent_)
                & boost::serialization::make_nvp("UserProperties", userProperties);
        }
    };




} }

#endif

