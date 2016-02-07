#include "ContractStressor.h"

namespace sjdObjects 
{
    ContractStressor::ContractStressor(const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                                       boost::shared_ptr<Contract> contract, 
                                       boost::shared_ptr<RatesEnvironment> rates,
                                       bool permanent) 
        : ObjectHandler::LibraryObject<sjd::ContractStressor>(properties, permanent)
    {
        boost::shared_ptr<sjd::Contract> sjdContract;
        contract->getLibraryObject(sjdContract);
        boost::shared_ptr<sjd::RatesEnvironment> sjdRatesEnvironment;
        rates->getLibraryObject(sjdRatesEnvironment);

        libraryObject_ = boost::shared_ptr<sjd::ContractStressor>(
            new sjd::ContractStressor(sjdContract, sjdRatesEnvironment));
    }

    bool ContractStressor::isOK()
    {
        return libraryObject_->isOK();
    }

    string ContractStressor::getErrorMessages() const
    {
        return libraryObject_->getErrorMessagesAsString();
    }

    vector<pair<string, double> > ContractStressor::getStressedValues()
    {
        return libraryObject_->getStressedValues();
    }
}