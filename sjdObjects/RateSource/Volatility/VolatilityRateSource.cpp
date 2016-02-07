#include "VolatilityRateSource.h"

namespace sjdObjects 
{        
   /*======================================================================================
    VolatilityRateSource

    =======================================================================================*/
    VolatilityRateSource::VolatilityRateSource(const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                                               boost::shared_ptr<sjd::VolatilityRateSource> vrs,
                                               bool permanent) : 
        ObjectHandler::LibraryObject<sjd::VolatilityRateSource>(properties, permanent) 
    {
        libraryObject_ = vrs;
    }

    Date VolatilityRateSource::getAnchorDate()
    {
        return libraryObject_->getAnchorDate();
    }

    double VolatilityRateSource::getVolatility(QuantLib::Date toDate) const
    {
        return libraryObject_->getVolatility(toDate);
    }
    
    double VolatilityRateSource::getVolatility(QuantLib::Date toDate, double strike, double forward) const
    {
        return libraryObject_->getVolatility(toDate, strike, forward);                
    };

    double VolatilityRateSource::getStandardDeviation(QuantLib::Date toDate) const
    {
        return libraryObject_->getStandardDeviation(toDate);
    }
    double VolatilityRateSource::getStandardDeviation(QuantLib::Date toDate, double strike, double forward) const
    {
        return libraryObject_->getStandardDeviation(toDate, strike, forward);

    }
    bool VolatilityRateSource::isOK() const
    {
        return libraryObject_->isOK();
    }

    string VolatilityRateSource::getErrorMessages() const
    {
        return libraryObject_->getErrorMessagesAsString();
    }

    bool VolatilityRateSource::isInRange(QuantLib::Date date) const
    {
        return libraryObject_->isInRange(date);
    }

    bool VolatilityRateSource::isInRange(Date date, double strike, double forward) const
    {
        return libraryObject_->isInRange(date, strike, forward);
    }


   /*======================================================================================
    DeterministicVolatilityRateSource

    =======================================================================================*/
    boost::shared_ptr<VolatilityRateSource> DeterministicVolatilityRateSource::rollForward(
                                                    const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                                                    Date toDate,
                                                    bool permanent)
    {
        boost::shared_ptr<sjd::DeterministicVolatilityRateSource> lo = 
            boost::dynamic_pointer_cast<sjd::DeterministicVolatilityRateSource>(libraryObject_); 
        boost::shared_ptr<sjd::VolatilityRateSource> rolledLO = lo->rollForward(toDate);
        return boost::shared_ptr<VolatilityRateSource>(new VolatilityRateSource(properties, rolledLO, permanent));
    }

    boost::shared_ptr<VolatilityRateSource> DeterministicVolatilityRateSource::parallelBump(
                                                    const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                                                    double spread,
                                                    bool permanent)
    {
        boost::shared_ptr<sjd::DeterministicVolatilityRateSource> lo = 
            boost::dynamic_pointer_cast<sjd::DeterministicVolatilityRateSource>(libraryObject_); 
        boost::shared_ptr<sjd::VolatilityRateSource> rolledLO = lo->parallelBump(spread);
        return boost::shared_ptr<VolatilityRateSource>(new VolatilityRateSource(properties, rolledLO, permanent));
    }


}