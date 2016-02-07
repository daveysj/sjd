#include "vo_VolatilityCurve.h"



namespace sjdObjects { namespace ValueObjects {

    const char* VolatilityCurveValueObject ::mPropertyNames[] = {
        // The two values below are not desired in the return value of ohObjectPropertyNames().
        // For now we just comment them out as this seems not to break anything.
        //"ClassName",
        //"ObjectId",
        "AnchorDate",
        "CurveDates",
        "Volatilities",
        "DayCounter",
        "Extrapolates",
        "Permanent"
    };

    std::set<std::string> VolatilityCurveValueObject ::mSystemPropertyNames(
        mPropertyNames, mPropertyNames + sizeof(mPropertyNames) / sizeof(const char*));

    const std::set<std::string>& VolatilityCurveValueObject ::getSystemPropertyNames() const {
        return mSystemPropertyNames;
    }

    std::vector<std::string> VolatilityCurveValueObject ::getPropertyNamesVector() const {
        std::vector<std::string> ret(
            mPropertyNames, mPropertyNames + sizeof(mPropertyNames) / sizeof(const char*));
        for (std::map<std::string, ObjectHandler::property_t>::const_iterator i = userProperties.begin();
            i != userProperties.end(); ++i)
            ret.push_back(i->first);
        return ret;
    }

    ObjectHandler::property_t VolatilityCurveValueObject ::getSystemProperty(const std::string& name) const {
        std::string nameUpper = boost::algorithm::to_upper_copy(name);
        if(strcmp(nameUpper.c_str(), "OBJECTID")==0)
            return objectId_;
        else if(strcmp(nameUpper.c_str(), "CLASSNAME")==0)
            return className_;
        else if(strcmp(nameUpper.c_str(), "ANCHORDATE")==0)
            return anchorDate_;
        else if(strcmp(nameUpper.c_str(), "CURVEDATES")==0)
            return curveDates_;
        else if(strcmp(nameUpper.c_str(), "VOLATILITIES")==0)
            return volatilities_;
        else if(strcmp(nameUpper.c_str(), "DAYCOUNTER")==0)
            return dayCounter_;
        else if(strcmp(nameUpper.c_str(), "EXTRAPOLATES")==0)
            return allowExtrapolation_;
        else if(strcmp(nameUpper.c_str(), "PERMANENT")==0)
            return permanent_;
        else
            OH_FAIL("Error: attempt to retrieve non-existent Property: '" + name + "'");
    }

    void VolatilityCurveValueObject::setSystemProperty(const std::string& name, const ObjectHandler::property_t& value) {
        std::string nameUpper = boost::algorithm::to_upper_copy(name);
        if(strcmp(nameUpper.c_str(), "OBJECTID")==0)
            objectId_ = boost::get<std::string>(value);
        else if(strcmp(nameUpper.c_str(), "CLASSNAME")==0)
            className_ = boost::get<std::string>(value);
        else if(strcmp(nameUpper.c_str(), "ANCHORDATE")==0)
            anchorDate_ = boost::get<std::string>(value);
        else if(strcmp(nameUpper.c_str(), "CURVEDATES")==0)
            curveDates_ = ObjectHandler::vector::convert2<ObjectHandler::property_t>(value, nameUpper);
        else if(strcmp(nameUpper.c_str(), "VOLATILITIES")==0)
            volatilities_ = ObjectHandler::vector::convert2<double>(value, nameUpper);
        else if(strcmp(nameUpper.c_str(), "DAYCOUNTER")==0)
            dayCounter_ = ObjectHandler::convert2<std::string>(value);
        else if(strcmp(nameUpper.c_str(), "EXTRAPOLATES")==0)
            allowExtrapolation_ = ObjectHandler::convert2<bool>(value);
        else if(strcmp(nameUpper.c_str(), "PERMANENT")==0)
            permanent_ = ObjectHandler::convert2<bool>(value);
        else
            OH_FAIL("Error: attempt to set non-existent Property: '" + name + "'");
    }

    VolatilityCurveValueObject::VolatilityCurveValueObject(
            const std::string& objectId,
            const ObjectHandler::property_t& anchorDate,
            const std::vector<ObjectHandler::property_t>& curveDates,
            const std::vector<double>& volatility,
            const std::string& dayCounter,
            //const ObjectHandler::property_t& interpolatorType,
            const bool& allowExtrapoation,
            bool permanent) :
        ObjectHandler::ValueObject(objectId, "sjdVolatilityCurve", permanent),
        anchorDate_(anchorDate),
        curveDates_(curveDates),
        volatilities_(volatility),
        dayCounter_(dayCounter),
        permanent_(permanent) {                  
            
    }



    /*======================================================================================
   CalendarAverageVolatilityMoneynessSurfaceValueObject
    
    =======================================================================================*/

    const char* CalendarAverageVolatilityMoneynessSurfaceValueObject::mPropertyNames[] = {
        "Permanent"
    };

    std::set<std::string> CalendarAverageVolatilityMoneynessSurfaceValueObject::mSystemPropertyNames(
        mPropertyNames, mPropertyNames + sizeof(mPropertyNames) / sizeof(const char*));

    const std::set<std::string>& CalendarAverageVolatilityMoneynessSurfaceValueObject::getSystemPropertyNames() const {
        return mSystemPropertyNames;
    }

    std::vector<std::string> CalendarAverageVolatilityMoneynessSurfaceValueObject::getPropertyNamesVector() const {
        std::vector<std::string> ret(
            mPropertyNames, mPropertyNames + sizeof(mPropertyNames) / sizeof(const char*));
        for (std::map<std::string, ObjectHandler::property_t>::const_iterator i = userProperties.begin();
            i != userProperties.end(); ++i)
            ret.push_back(i->first);
        return ret;
    }

    ObjectHandler::property_t CalendarAverageVolatilityMoneynessSurfaceValueObject::getSystemProperty(const std::string& name) const {
        std::string nameUpper = boost::algorithm::to_upper_copy(name);
        if(strcmp(nameUpper.c_str(), "OBJECTID")==0)
            return objectId_;
        else if(strcmp(nameUpper.c_str(), "CLASSNAME")==0)
            return className_;
        else if(strcmp(nameUpper.c_str(), "PERMANENT")==0)
            return permanent_;
        else
            OH_FAIL("Error: attempt to retrieve non-existent Property: '" + name + "'");
    }

    void CalendarAverageVolatilityMoneynessSurfaceValueObject::setSystemProperty(const std::string& name, const ObjectHandler::property_t& value) {
        std::string nameUpper = boost::algorithm::to_upper_copy(name);
        if(strcmp(nameUpper.c_str(), "OBJECTID")==0)
            objectId_ = boost::get<std::string>(value);
        else if(strcmp(nameUpper.c_str(), "CLASSNAME")==0)
            className_ = boost::get<std::string>(value);
        else if(strcmp(nameUpper.c_str(), "PERMANENT")==0)
            permanent_ = ObjectHandler::convert2<bool>(value);
        else
            OH_FAIL("Error: attempt to set non-existent Property: '" + name + "'");
    }

    CalendarAverageVolatilityMoneynessSurfaceValueObject::CalendarAverageVolatilityMoneynessSurfaceValueObject(
            const std::string& objectId,
            bool permanent) :
        ObjectHandler::ValueObject(objectId, "createCalAveVolSurface", permanent),
        permanent_(permanent) {                  
            
    }


}}