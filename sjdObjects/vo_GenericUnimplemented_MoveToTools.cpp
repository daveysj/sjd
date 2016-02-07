#include "vo_GenericUnimplemented.h"

namespace sjdObjects { namespace ValueObjects {

    /*======================================================================================
   GenericUnimplementedValueObject
    
    =======================================================================================*/

    const char* GenericUnimplementedValueObject::mPropertyNames[] = {
        "Permanent"
    };

    std::set<std::string> GenericUnimplementedValueObject::mSystemPropertyNames(
        mPropertyNames, mPropertyNames + sizeof(mPropertyNames) / sizeof(const char*));

    const std::set<std::string>& GenericUnimplementedValueObject::getSystemPropertyNames() const {
        return mSystemPropertyNames;
    }

    std::vector<std::string> GenericUnimplementedValueObject::getPropertyNamesVector() const {
        std::vector<std::string> ret(
            mPropertyNames, mPropertyNames + sizeof(mPropertyNames) / sizeof(const char*));
        for (std::map<std::string, ObjectHandler::property_t>::const_iterator i = userProperties.begin();
            i != userProperties.end(); ++i)
            ret.push_back(i->first);
        return ret;
    }

    ObjectHandler::property_t GenericUnimplementedValueObject::getSystemProperty(const std::string& name) const {
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

    void GenericUnimplementedValueObject::setSystemProperty(const std::string& name, const ObjectHandler::property_t& value) {
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

    GenericUnimplementedValueObject::GenericUnimplementedValueObject(
            const std::string& objectId,
            bool permanent) :
        ObjectHandler::ValueObject(objectId, "createCalAveVolSurface", permanent),
        permanent_(permanent) {                  
            
    }


}}