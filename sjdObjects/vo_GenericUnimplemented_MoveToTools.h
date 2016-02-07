#ifndef SJD_UNIMPLEMENTEDVALUEOBJECT
#define SJD_UNIMPLEMENTEDVALUEOBJECT

#include <oh/valueobject.hpp>
#include <string>
#include <vector>
#include <set>
#include <boost/serialization/map.hpp>
#include <boost/algorithm/string/case_conv.hpp>

namespace sjdObjects { namespace ValueObjects {



    /*======================================================================================
   GenericUnimplementedValueObject
    
    Value objects are only important for serialization. I have chosen not to implement most
    of these in the interest of getting something working quickly
    =======================================================================================*/
    class GenericUnimplementedValueObject : public ObjectHandler::ValueObject {
        friend class boost::serialization::access;
    public:
        GenericUnimplementedValueObject() {}
        GenericUnimplementedValueObject(const std::string& objectId, bool permanent);

        const std::set<std::string>& getSystemPropertyNames() const;
        std::vector<std::string> getPropertyNamesVector() const;
        ObjectHandler::property_t getSystemProperty(const std::string&) const;
        void setSystemProperty(const std::string& name, const ObjectHandler::property_t& value);

    protected:
        static const char* mPropertyNames[];
        static std::set<std::string> mSystemPropertyNames;

        bool permanent_;

        template<class Archive>
        void serialize(Archive& ar, const unsigned int) {
        boost::serialization::void_cast_register<qlDiscountCurve, ObjectHandler::ValueObject>(this, this);
            ar  & boost::serialization::make_nvp("ObjectId", objectId_)
                & boost::serialization::make_nvp("Permanent", Permanent_)
                & boost::serialization::make_nvp("UserProperties", userProperties);
        }
    };

}}

#endif