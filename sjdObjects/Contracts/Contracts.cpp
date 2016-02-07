#include "Contracts.h"

#include <boost/algorithm/string.hpp> 

namespace sjdObjects 
{

   /*======================================================================================
   Contract

    =======================================================================================*/
    bool Contract::isOK(boost::shared_ptr<RatesEnvironment> re) const
    {
        boost::shared_ptr<sjd::RatesEnvironment> sjdRe;
        re->getLibraryObject(sjdRe);
        libraryObject_->validate(sjdRe);
        return libraryObject_->isOK();
    }

    string Contract::getErrorMessages() const
    {
        return libraryObject_->getErrorMessagesAsString();
    }

    double Contract::getValue(boost::shared_ptr<RatesEnvironment> re)
    {
        boost::shared_ptr<sjd::RatesEnvironment> sjdRe;
        re->getLibraryObject(sjdRe);
        return libraryObject_->getValue(sjdRe);
    }

    string Contract::toString(boost::shared_ptr<RatesEnvironment> re)
    {
        boost::shared_ptr<sjd::RatesEnvironment> sjdRE;
        re->getLibraryObject(sjdRE);
        return libraryObject_->toString(sjdRE);
    }


    /*======================================================================================
   FixedCashflowContract

    =======================================================================================*/
    FixedCashflowContract::FixedCashflowContract(const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                                     Date cashflowDate, 
                                     double volume, 
                                     sjd::BuySell buySell,
                                     bool permanent) 
         : Contract(properties, permanent) 
    {
        libraryObject_ = boost::shared_ptr<sjd::Contract>(new 
                            sjd::FixedCashflow(cashflowDate, volume, buySell));
    }
                
    /*======================================================================================
   ForwardContract

    =======================================================================================*/
    ForwardContract::ForwardContract(const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                             Date rateSetDate, 
                            Date settlement, 
                            double strike, 
                            double volume,
                            sjd::BuySell buySell,
                            bool permanent) 
      : Contract(properties, permanent) 
    {
        libraryObject_ = boost::shared_ptr<sjd::Contract>(new 
                            sjd::EuropeanForward(rateSetDate, settlement, strike, volume, buySell));
    }
      
   /*======================================================================================
    AsianForwardContract

    =======================================================================================*/
    AsianForwardContract::AsianForwardContract(const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                                     Calendar calendar, 
                                    Date startOfAveraging, 
                                    Date endOfAveraging, 
                                    Date settlement, 
                                    double strike, 
                                    double volume, 
                                    sjd::BuySell buySell, 
                                    bool permanent) 
         : Contract(properties, permanent) 
    {
        libraryObject_ = boost::shared_ptr<sjd::Contract>(new 
                            sjd::AsianForward(calendar, startOfAveraging, endOfAveraging, settlement, strike, volume, buySell));
    }

   /*======================================================================================
   OptionContract

    =======================================================================================*/
    OptionContract::OptionContract(const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                           Date optionMaturity, 
                                   Date optionSettlement, 
                                   double strike, 
                                   sjd::PutCall type,
                                   double volume,
                                   Date premiumPaymentDate, 
                                   double premiumAmount, 
                                   sjd::BuySell buySell,
                                   bool permanent) 
      : Contract(properties, permanent) 
    {
      libraryObject_ = boost::shared_ptr<sjd::Contract>(new 
                     sjd::EuropeanOption(optionMaturity, 
                                                optionSettlement, 
                                                strike, 
                                                type, 
                                                volume, 
                                                premiumPaymentDate, 
                                                premiumAmount, 
                                                buySell));
    }

    /*======================================================================================
    AsianOptionContract

    =======================================================================================*/
    AsianOptionContract::AsianOptionContract(const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                                         Calendar calendar,
                                             Date startOfAveraging, 
                                             Date endOfAveraging, 
                                             Date optionSettlement, 
                                             double strike, 
                                             sjd::PutCall type,
                                             double volume,
                                             Date premiumPaymentDate, 
                                             double premiumAmount, 
                                             sjd::BuySell buySell,
                                             bool permanent) : Contract(properties, permanent) 
    {
      libraryObject_ = boost::shared_ptr<sjd::Contract>(new 
                     sjd::AsianOption(calendar, 
                                             startOfAveraging, 
                                             endOfAveraging, 
                                             optionSettlement, 
                                             strike, 
                                             type, 
                                             volume, 
                                             premiumPaymentDate, 
                                             premiumAmount, 
                                             buySell));
    }
                        
    /*======================================================================================
    MultipleInstrumentContract

    =======================================================================================*/
    MultipleInstrumentContract::MultipleInstrumentContract(
                                    const boost::shared_ptr<ObjectHandler::ValueObject>& properties,
                                 vector<boost::shared_ptr<Contract>> inputs,
                                    bool permanent) : Contract(properties, permanent) 
    {
        vector<boost::shared_ptr<sjd::Contract>> sjdInputs;
        for (size_t i = 0; i < inputs.size(); ++i) 
        {
            boost::shared_ptr<sjd::Contract> ec;
            inputs[i]->getLibraryObject(ec);
            sjdInputs.push_back(ec);
        }
        libraryObject_ = boost::shared_ptr<sjd::Contract>(new sjd::MultipleInstrumentContract(sjdInputs));
    }
}