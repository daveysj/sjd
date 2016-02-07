#include "DiscountRateSource.h"


namespace sjdObjects 
{
   /*======================================================================================
   DiscountRateSource

    =======================================================================================*/
    DiscountRateSource::DiscountRateSource(const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                                           boost::shared_ptr<sjd::DiscountRateSource> drs,
                                           bool permanent) : 
        ObjectHandler::LibraryObject<sjd::DiscountRateSource>(properties, permanent) 
    {
        libraryObject_ = drs;
    }


    vector<double> DiscountRateSource::getDiscountFactor(const vector<QuantLib::Date>& dates) const
    {
        return libraryObject_->getDiscountFactor(dates);
    }

   bool DiscountRateSource::isOK() const
   {
      return libraryObject_->isOK();
   }

    string DiscountRateSource::getErrorMessages() const
    {
      return libraryObject_->getErrorMessagesAsString();
    }

    bool DiscountRateSource::isInRange(QuantLib::Date date) const
    {
        return libraryObject_->isInRange(date);
    }

}