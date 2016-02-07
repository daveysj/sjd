#include "RatesEnvironment.h"


namespace sjdObjects 
{
   
    /*======================================================================================
    RatesEnvironment

    =======================================================================================*/
    RatesEnvironment::RatesEnvironment(const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                                       Date anchorDate,
                                       boost::shared_ptr<sjdObjects::DiscountRateSource> drs,
                                       boost::shared_ptr<sjdObjects::DiscountRateSource> foreigndrs,
                                       boost::shared_ptr<sjdObjects::ForwardRateSource> frs,
                                       boost::shared_ptr<sjdObjects::VolatilityRateSource> vrs,
                                       boost::shared_ptr<sjdObjects::FixingRateSource> fixingrs,
                                       bool permanent) :  ObjectHandler::LibraryObject<sjd::RatesEnvironment>(properties, permanent) 
    {
        libraryObject_ = boost::shared_ptr<sjd::RatesEnvironment>(new sjd::RatesEnvironment(anchorDate));
        if (drs) 
        {
            boost::shared_ptr<sjd::DiscountRateSource> sjdDrs;
            drs->getLibraryObject(sjdDrs);
            libraryObject_->setDRS(sjdDrs);
        }
        if (foreigndrs) 
        {
            boost::shared_ptr<sjd::DiscountRateSource> sjdForeignDrs;
            foreigndrs->getLibraryObject(sjdForeignDrs);
            libraryObject_->setForeignDRS(sjdForeignDrs);
        }
        if (frs) 
        {
            boost::shared_ptr<sjd::ForwardRateSource> sjdFrs;
            frs->getLibraryObject(sjdFrs);
            libraryObject_->setFRS(sjdFrs);
        }
        if (vrs) 
        {
            boost::shared_ptr<sjd::VolatilityRateSource> sjdVrs;
            vrs->getLibraryObject(sjdVrs);
            libraryObject_->setVRS(sjdVrs);
        }
        if (fixingrs) 
        {
            boost::shared_ptr<sjd::FixingRateSource> sjdFixingRS;
            fixingrs->getLibraryObject(sjdFixingRS);
            libraryObject_->setFixingRS(sjdFixingRS);
        }
    }

    RatesEnvironment::RatesEnvironment(const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                        boost::shared_ptr<sjd::RatesEnvironment> re,
                        bool permanent) :  ObjectHandler::LibraryObject<sjd::RatesEnvironment>(properties, permanent) 
    {
        libraryObject_ = re;
    }

    bool RatesEnvironment::isOK()
    {
        return libraryObject_->isOK();
    }

    string RatesEnvironment::getErrorMessages() const
    {
        return libraryObject_->getErrorMessagesAsString();
    }

    Date RatesEnvironment::getAnchorDate()
    {
        return libraryObject_->getAnchorDate();
    }

    boost::shared_ptr<sjd::ForwardRateSource> RatesEnvironment::getFRS()
    {
        return libraryObject_->getFRS();
    }

    boost::shared_ptr<sjd::DiscountRateSource> RatesEnvironment::getDRS()
    {
        return libraryObject_->getDRS();
    }

    boost::shared_ptr<sjd::DiscountRateSource> RatesEnvironment::getForeignDRS()
    {
        return libraryObject_->getForeignDRS();
    }

    boost::shared_ptr<sjd::VolatilityRateSource> RatesEnvironment::getVRS()
    {
        return libraryObject_->getVRS();
    }

    boost::shared_ptr<sjd::FixingRateSource> RatesEnvironment::getFixingRS()
    {
        return libraryObject_->getFixingRS();
    }

    bool RatesEnvironment::hasFRS()
    {
        return libraryObject_->hasFRS();
    }

    bool RatesEnvironment::hasDRS()
    {
        return libraryObject_->hasDRS();
    }

    bool RatesEnvironment::hasForeignDRS()
    {
        return libraryObject_->hasForeignDRS();
    }

    bool RatesEnvironment::hasVRS()
    {
        return libraryObject_->hasVRS();
    }

    bool RatesEnvironment::hasFixingRS()
    {
        return libraryObject_->hasFixingRS();
    }

}