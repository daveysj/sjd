#include "Contracts.h"
#include <boost/lexical_cast.hpp>

namespace sjd 
{
   /*======================================================================================
   Contract

    =======================================================================================*/
    void Contract::validate()
    {
        errorTracking->clearErrorMessages();
    }

    void Contract::validate(boost::shared_ptr<RatesEnvironment> re)
    {
        this->validate();
        if (!re->isOK())
        {
            errorTracking->populateErrorMessage(re->getErrorMessages());
        }
    }

    bool Contract::isOK()
    {      
        return !errorTracking->getHasErrors();
    }

    vector<string> Contract::getErrorMessages() const
    {      
        return errorTracking->getErrorMessages();
    }

    string Contract::getErrorMessagesAsString() const
    {
        return errorTracking->getErrorMessagesAsString();
    }

    string Contract::toString()
    {
        stringstream description;
        validate();
        if (!isOK())
        {
            description << "Contract has errors,";
            vector<string> errorMessages = getErrorMessages();
            for (size_t i = 0; i < errorMessages.size(); ++i) 
            {
                description << errorMessages[i] << ",";
            }
            return description.str();
        }
        description << toStringContract();
        return description.str();
    }

    string Contract::toString(boost::shared_ptr<RatesEnvironment> re)
    {
        validate(re);
        stringstream description;
        if (!isOK())
        {
            description << "Contract has errors,";
            vector<string> errorMessages = getErrorMessages();
            for (size_t i = 0; i < errorMessages.size(); ++i) 
            {
                description << errorMessages[i] << ",";
            }
            return description.str();
        }
        description << toStringContract();
        description << toStringRates(re);
        return description.str() ;
    }

    /*======================================================================================
    SingleInstrumentContract

    =======================================================================================*/
    void SingleInstrumentContract::validate()
    {
        Contract::validate();
        if (maturityDate > settlementDate) 
        {
            errorTracking->populateErrorMessage("Maturity date after settlement date");
        }
        if (hasPremium && (getPremiumPaymentDate() > getSettlementDate()))
        {
            errorTracking->populateErrorMessage(
                "Premium payment date after settlement date. Consider using a cash flow and an option without premium");
        }
        if (hasPremium && (premiumAmount < -1e-14))
        {
            errorTracking->populateErrorMessage("Premium Amount must be stored as a non-negative number");
        }
    }

    void SingleInstrumentContract::validate(boost::shared_ptr<RatesEnvironment> re)
    {
        Contract::validate(re);
        if (!re->hasDRS()) 
        {
            errorTracking->populateErrorMessage("No discounting rate source");
        }
        else 
        {
            // can't use if (!drs->isInRange(settlementDate)) 
            // because there are legitimate instances where we may value the option after settlement
            if (!(re->getDRS())->extendsTo(settlementDate))  
            {
            errorTracking->populateErrorMessage("Discounting rate source does not extend to settlement");
            }
        }
    }

    bool SingleInstrumentContract::getHasPremium() const
    {
        return hasPremium;
    }

    Date SingleInstrumentContract::getPremiumPaymentDate() const
    {
        return premiumPaymentDate;
    }

    void SingleInstrumentContract::setPremiumPaymentDate(Date paymentDate)
    {
        premiumPaymentDate = paymentDate;
        if (premiumAmount > 1e-14)
        {
            hasPremium = true;
        }
        else 
        {
            hasPremium = false;
        }
        cachedContractValue = false;
    }

    void SingleInstrumentContract::setPremiumAmount(double premium)
    {
        premiumAmount = abs(premium);
        if (premiumAmount > 1e-14)
        {
            hasPremium = true;
        }
        else 
        {
            hasPremium = false;
        }
        cachedContractValue = false;
    }

    double SingleInstrumentContract::getPremiumAmount() const
    {
        return premiumAmount;
    }

    void SingleInstrumentContract::setMaturityDate(Date date)
    {
        maturityDate = date;
        cachedContractValue = false;
    }

    Date SingleInstrumentContract::getMaturityDate() const
    {
        return maturityDate;
    }
    
    void SingleInstrumentContract::setSettlementDate(Date date)
    {
        settlementDate = date;
        cachedContractValue = false;
    }

    Date SingleInstrumentContract::getSettlementDate() const
    {
        return settlementDate;
    }

    void SingleInstrumentContract::setBuySell(BuySell bs)
    {
        buySell = bs;
        if (buySell == BUY)
        {
            buySellAsDouble = 1;
        }
        else if (buySell == SELL)
        {
            buySellAsDouble = -1;
        }
        else 
        {
            buySellAsDouble = 0;
        }
        cachedContractValue = false;
    }

    BuySell SingleInstrumentContract::getBuySell() const
    {
        return buySell;
    }

   double SingleInstrumentContract::getBuySellAsDouble() const
   {
      return buySellAsDouble;
   }

    double SingleInstrumentContract::getVolume() const
    {
        return volume;
    }

    void SingleInstrumentContract::setVolume(double N)
    {
        volume = abs(N);
        cachedContractValue = false;
    }

    string SingleInstrumentContract::toStringContract()
    {
        stringstream descriptionStream;
        descriptionStream << errorTracking->getClassName()
            << ",Set," <<  QuantLib::io::iso_date(getSettlementDate())
            << ",Mat," <<  QuantLib::io::iso_date(getMaturityDate())
            << "," << convertBuySellToString(buySell)
            << ",Volume," << fixed << setprecision(2) << volume;
        if (hasPremium) 
        {
            descriptionStream << ",Prem," << fixed << setprecision(4) << premiumAmount
                << ",PremDate," << QuantLib::io::iso_date(getPremiumPaymentDate());
        }
        else 
        {
            descriptionStream << ",NoPrem,0,PremDate,01/01/1901";
        }
        return descriptionStream.str();
    }

    string SingleInstrumentContract::toStringRates(boost::shared_ptr<RatesEnvironment> re)
    {
        stringstream ratesDescriptionStream;
        ratesDescriptionStream << ",MtM," << fixed << setprecision(4) << getValue(re)
                               << ",DF(set)," << fixed << setprecision(6) 
                               << (re->getDRS())->getDiscountFactor(getSettlementDate());
        return ratesDescriptionStream.str();
    }

    /*======================================================================================
    SingleDerivativeContract

    =======================================================================================*/
    void SingleDerivativeContract::validate()
    {
        SingleInstrumentContract::validate();
        if (strike < 0) 
        {
           errorTracking->populateErrorMessage("Strike is negative");      
        }
    }

    void SingleDerivativeContract::validate(boost::shared_ptr<RatesEnvironment> re)
    {
        SingleInstrumentContract::validate(re);
        if (!re->hasFRS()) 
        {
            errorTracking->populateErrorMessage("No forward rate source");
        }
      else 
        {
         // NB This will need to be overwritten for Asian derivaitves which only require fwds to the
         // averaging dates
         if (!(re->getFRS())->extendsTo(getSettlementDate())) 
         {
            errorTracking->populateErrorMessage("Contract settlement not in forward rate source range");
         }
      }
    }

    void SingleDerivativeContract::setStrike(double X)
    {
        strike = X;
        cachedContractValue = false;
    }

    double SingleDerivativeContract::getStrike() const
    {
        return strike;
    }

    string SingleDerivativeContract::toStringContract()
    {
        stringstream descriptionStream;
        descriptionStream << SingleInstrumentContract::toStringContract();
        descriptionStream << ",Strike," << fixed << setprecision(4) << strike;
        return descriptionStream.str();
    }

    string SingleDerivativeContract::toStringRates(boost::shared_ptr<RatesEnvironment> re)
    {
        stringstream descriptionStream;
        descriptionStream << SingleInstrumentContract::toStringRates(re);
        if (isOK())
        {
            descriptionStream << ",Fwd(Set)," << fixed << setprecision(4) 
                              << (re->getFRS())->getForward(getSettlementDate());
        }
        return descriptionStream.str();
    }

    /*======================================================================================
    SingleOptionContract

    =======================================================================================*/
    void SingleOptionContract::validate()
    {
        SingleDerivativeContract::validate();
        if ((type != CALL) && (type != PUT))
        {
            errorTracking->populateErrorMessage("Type must be either CALL or PUT");
        }
    }

    void SingleOptionContract::validate(boost::shared_ptr<RatesEnvironment> re)
    {      
        SingleDerivativeContract::validate(re);
        if (!re->hasVRS()) 
        {
            errorTracking->populateErrorMessage("No volatiltiy rate source");
        }
        else 
        {
            if (!(re->getVRS())->extendsTo(getSettlementDate())) 
             {
                errorTracking->populateErrorMessage("Contract settlement not in volatility rate source range");
             }
        }
    }

    double SingleOptionContract::getPayoff(double forSpot)
    {
        double premium = 0;
        if (getPremiumPaymentDate() > getMaturityDate()) 
        {
            premium = getPremiumAmount();
        }

        if (type == PUT) 
        {
            return (max(0.0, getStrike() - forSpot) - premium) * getVolume() * getBuySellAsDouble();
        }
        else 
        {
            return (max(0.0, forSpot - getStrike()) - premium) * getVolume() * getBuySellAsDouble();
        }
    }

    double SingleOptionContract::getValue(boost::shared_ptr<RatesEnvironment> re)
    {
        if (valueCalcuated && (re == cachedRE))
        {
            return cachedContractValue;
        }
        Date valueDate = (re->getAnchorDate());
        if (valueDate > getSettlementDate())
        {
            return 0;
        }
        double premium = getOptionValue(re);
        if (valueDate <= getPremiumPaymentDate()) 
        {
            double df = (re->getDRS())->getDiscountFactor(getPremiumPaymentDate());
            premium -= getPremiumAmount()  * getVolume() * df;
        }
        cachedRE = re;
        valueCalcuated = true;
        cachedContractValue = getBuySellAsDouble() * premium;
        return cachedContractValue;
    }

   void SingleOptionContract::setType(PutCall pc)
    {
        type = pc;
        cachedContractValue = false;
    }

    PutCall SingleOptionContract::getType() const
    {
        return type;
    }


    string SingleOptionContract::toStringContract()
    {
        stringstream descriptionStream; 
        descriptionStream << SingleDerivativeContract::toStringContract();
        descriptionStream <<"," << convertPutCallToString(type);
        return descriptionStream.str();
    }

    string SingleOptionContract::toStringRates(boost::shared_ptr<RatesEnvironment> re)
    {
        stringstream descriptionStream; 
        descriptionStream << SingleDerivativeContract::toStringRates(re);
        if (isOK())
        {
            descriptionStream <<",Vol(Mat)," << fixed << setprecision(6) 
                              << (re->getVRS())->getVolatility(getMaturityDate());
        }
        return descriptionStream.str();
    }

    /*======================================================================================
    AsianContract

    =======================================================================================*/
    void AsianContract::validate()
    {
        if (orderedAveragingDates.size() < 1) 
        {
            errorTracking->populateErrorMessage("Averaging Dates not set");
        }
    }

    void AsianContract::validate(boost::shared_ptr<RatesEnvironment> re)
    {
        AsianContract::validate();
        Date valueDate = (re->getAnchorDate());
        if (!re->hasFixingRS()) 
        {
            errorTracking->populateErrorMessage("No Fixing rate source");
        }
        else if (!(re->getFixingRS())->isOK()) 
        {
            errorTracking->populateErrorMessage((re->getFixingRS())->getErrorMessages());
        }

        vector<Date> fixingDates, forwardDates;
        getFixingAndForwardDates(valueDate, fixingDates, forwardDates);

        for (size_t i = 0; i < fixingDates.size(); ++i) 
        {
            Date d = fixingDates[i];
            if (!(re->getFixingRS())->containsARateSetOn(d)) 
            {
                stringstream descriptionStream;
                descriptionStream << "Fixing Rate Source does not contain a fixing rate on "
                          << QuantLib::io::iso_date(d);
                errorTracking->populateErrorMessage(descriptionStream.str());
            }
        }
    }

    vector<string> AsianContract::getAsianContractErrorMessages()
    {
        return errorTracking->getErrorMessages();
    }

    void AsianContract::setAveragingDates(set<Date> dates)
    {
        orderedAveragingDates = vector<Date>(dates.begin(), dates.end());
    }

    vector<Date> AsianContract::getAveragingDates() const
    {
        return orderedAveragingDates;
    }

    set<Date> AsianContract::getAllBusinessDaysBetween(Date startDate, Date endDate, Calendar calendar)
    {            
        Date averagingStart;
        if (calendar.isBusinessDay(startDate)) 
        {
            averagingStart = startDate;
        }
        else 
        {
            averagingStart = calendar.advance(startDate, 1, Days);
        }
        Date lastDateInAveragingMonth;
        if (calendar.isBusinessDay(endDate))
        {
            lastDateInAveragingMonth = endDate;
        }
        else 
        {
            lastDateInAveragingMonth = calendar.advance(endDate, -1, Days);
        }
        set<Date> averagingDates;
        Date tmp = averagingStart;
        while (tmp <= endDate) 
        {
            averagingDates.insert(tmp);
            tmp = calendar.advance(tmp, 1, Days);
        }
        return averagingDates;
    }

    void AsianContract::getFixingAndForwardDates(Date valuationDate, 
                                                 vector<Date> &fixingDates, 
                                                 vector<Date> &forwardDates)
    {
        if (valuationDate < orderedAveragingDates[0])
        {
            forwardDates = orderedAveragingDates;
            fixingDates = vector<Date>(0);
            return;
        }
        fixingDates.clear();
        forwardDates.clear();
        for (size_t i = 0; i < orderedAveragingDates.size(); ++i) 
        {
            if (orderedAveragingDates[i] <= valuationDate) 
            {
                fixingDates.push_back(orderedAveragingDates[i]);
            }
            else 
            {
                forwardDates.push_back(orderedAveragingDates[i]);
            }
        }
    }

    void AsianContract::getFixingAndForwardRates(Date valueDate, 
                                              vector<double> &fixingRates, 
                                              vector<double> &forwardRates,
                                              boost::shared_ptr<RatesEnvironment> re)
    {
        vector<Date> fixingDates, forwardDates;
        getFixingAndForwardDates(valueDate, fixingDates, forwardDates);        
        fixingRates = (re->getFixingRS())->getRateSetOn(fixingDates);
        forwardRates = (re->getFRS())->getForward(forwardDates);        
        return;
    }


   /*======================================================================================
    MultipleInstrumentContract

    Base class for derivatives may contain more than one underlying contract
    =======================================================================================*/
    MultipleInstrumentContract::MultipleInstrumentContract(MultipleInstrumentContract &multipleInstrumentContract)
    {
        setInputVariables();
        contracts = multipleInstrumentContract.contracts;        
    }

    MultipleInstrumentContract* MultipleInstrumentContract::clone()
    {
        MultipleInstrumentContract *multipleInstrumentContract = new MultipleInstrumentContract(*this);
        return multipleInstrumentContract;
    }
    void MultipleInstrumentContract::setInputVariables()
    {
        errorTracking = boost::shared_ptr<sjdTools::ErrorTracking>(new sjdTools::ErrorTracking("MultipleInstrumentContract"));
    }

    MultipleInstrumentContract::MultipleInstrumentContract(vector<boost::shared_ptr<SingleInstrumentContract>> inputs)
    {
        setInputVariables();
        contracts = inputs;
    };

    MultipleInstrumentContract::MultipleInstrumentContract(vector<boost::shared_ptr<Contract>> inputs)
    {
        setInputVariables();
        vector<boost::shared_ptr<SingleInstrumentContract>> inputsAsSingleInstruments = 
            getSingleInstrumentContracts(inputs);
        if (!isOK()) 
        {
            contracts = vector<boost::shared_ptr<SingleInstrumentContract>>(0);
            errorTracking->populateErrorMessage("Cannot extract single instruments from input contracts");
        }
        else 
        {
            contracts = inputsAsSingleInstruments;
        }
    };

    void MultipleInstrumentContract::validate()
    {
        if (contracts.size() == 0)
        {
            errorTracking->populateErrorMessage(", contains no contracts");
            return;
        }

        Contract::validate();

        for (size_t i = 0; i < contracts.size(); ++i) 
        {
            contracts[i]->validate();
            if (!contracts[i]->isOK())
            {
                errorTracking->populateErrorMessage(contracts[i]->getErrorMessages());
            }
        }
    }

    void MultipleInstrumentContract::validate(boost::shared_ptr<RatesEnvironment> re)
    {
        validate();
        Contract::validate(re);
        for (size_t i = 0; i < contracts.size(); ++i) 
        {
            contracts[i]->validate(re);
            if (!contracts[i]->isOK())
            {
                errorTracking->populateErrorMessage(contracts[i]->getErrorMessages());
            }
        }
    }

    double MultipleInstrumentContract::getValue(boost::shared_ptr<RatesEnvironment> re)
    {
        double value = 0;
        for (size_t i = 0; i < contracts.size(); ++i) 
        {
            value += contracts[i]->getValue(re);
        }
        return value;
    }

    string MultipleInstrumentContract::toString(boost::shared_ptr<RatesEnvironment> re)
    {
        validate(re);
        stringstream description;
        if (!isOK())
        {
            description << "Contract has errors,";
            vector<string> errorMessages = getErrorMessages();
            for (size_t i = 0; i < errorMessages.size(); ++i) 
            {
                description << errorMessages[i] << ",";
            }
            return description.str();
        }
        description << "Net MtM," << fixed << setprecision(4) << getValue(re) << "\n";
        description << toStringRates(re);
        return description.str() ;
    }

    string MultipleInstrumentContract::toStringContract()
    {
        string description;
        for (size_t i = 0; i < contracts.size(); ++i)
        {
            description += contracts[i]->toStringContract() +"\n";
        }
        return description;
    }

    string MultipleInstrumentContract::toStringRates(boost::shared_ptr<RatesEnvironment> re)
    {
        string description;
        for (size_t i = 0; i < contracts.size(); ++i)
        {
            description += contracts[i]->toStringContract() + contracts[i]->toStringRates(re) +"\n";
        }
        return description;
    }

    vector<boost::shared_ptr<SingleInstrumentContract>> MultipleInstrumentContract::getUnderlyingSingleInstrumentContracts()
    {
        vector<boost::shared_ptr<SingleInstrumentContract>> clonedContracts;
        for (size_t i = 0; i < contracts.size(); ++i) 
        {
            clonedContracts.push_back(boost::shared_ptr<SingleInstrumentContract>(contracts[i]->clone()));
        }
        return clonedContracts;
    }

    vector<boost::shared_ptr<SingleInstrumentContract>> MultipleInstrumentContract::getSingleInstrumentContracts(
                                                                            vector<boost::shared_ptr<Contract>> inputs)
    {
        vector<boost::shared_ptr<SingleInstrumentContract>> inputsAsSingleInstruments;
        for (size_t i = 0; i < inputs.size(); ++i) 
        {
            if (boost::dynamic_pointer_cast<SingleInstrumentContract>(inputs[i]) != 0) 
            {
                inputsAsSingleInstruments.push_back(boost::dynamic_pointer_cast<SingleInstrumentContract>(inputs[i]));
            }
            else if (boost::dynamic_pointer_cast<MultipleInstrumentContract>(inputs[i]) != 0) 
            {
                vector<boost::shared_ptr<SingleInstrumentContract>> underlyingContracts = 
                    boost::dynamic_pointer_cast<MultipleInstrumentContract>(inputs[i])->
                    getUnderlyingSingleInstrumentContracts();

                for (size_t j = 0; j < underlyingContracts.size(); ++j) 
                {
                    inputsAsSingleInstruments.push_back(underlyingContracts[j]);
                }
            }
        }
        return inputsAsSingleInstruments;
    }

}