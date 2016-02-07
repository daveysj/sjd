#include "FixingRateSource.h"


namespace sjdObjects 
{
    /*======================================================================================
    FixingRateSource

    =======================================================================================*/
    FixingRateSource::FixingRateSource(const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                                       boost::shared_ptr<sjd::RollingFixingRateSource> drs,
                                       bool permanent) :
        ObjectHandler::LibraryObject<sjd::RollingFixingRateSource>(properties, permanent) 
    {
        libraryObject_ = drs;
    }

    vector<double> FixingRateSource::getRateSetOn(const vector<QuantLib::Date>& dates) const
    {
        return libraryObject_->getRateSetOn(dates);
    }

    bool FixingRateSource::isOK() const
    {
        return libraryObject_->isOK();
    }

    string FixingRateSource::getErrorMessages() const
    {
        return libraryObject_->errorTracking->getErrorMessagesAsString();
    }

    bool FixingRateSource::containsARateSetOn(QuantLib::Date date) const
    {
        return libraryObject_->containsARateSetOn(date);
    }
}