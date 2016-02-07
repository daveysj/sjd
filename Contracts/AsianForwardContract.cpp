#include "AsianForwardContract.h"


namespace sjd {


   /*======================================================================================
   AsianForward

    =======================================================================================*/
    void AsianForward::constructorCommon(set<Date> averagingDatesInput, 
                                        Date settlementInput, 
                                        double strikeInput, 
                                        double volumeInput,
                                        BuySell buySellInput)
    {
      ForwardContract::errorTracking = boost::shared_ptr<sjdTools::ErrorTracking>(new sjdTools::ErrorTracking("AsianForward"));
      AsianContract::errorTracking = boost::shared_ptr<sjdTools::ErrorTracking>(new sjdTools::ErrorTracking("AsianForward"));
        setAveragingDates(averagingDatesInput);
        setSettlementDate(settlementInput);
        setStrike(strikeInput);
        setVolume(volumeInput);
        setBuySell(buySellInput);
        setPremiumAmount(0);
        setPremiumPaymentDate(settlementInput);
    }

    AsianForward::AsianForward(set<Date> averagingDates, 
                               Date settlement, 
                               double strike, 
                               double volume, 
                               BuySell buySell)
    {
        constructorCommon(averagingDates, settlement, strike, volume, buySell);
    }

    AsianForward::AsianForward(Calendar calendar, 
                               Date startOfAveraging, 
                               Date endOfAveraging, 
                               Date settlement, 
                               double strike, 
                               double volume, 
                               BuySell buySell)
    {
        set<Date> averagingDates = getAllBusinessDaysBetween(startOfAveraging, endOfAveraging, calendar);
        constructorCommon(averagingDates, settlement, strike, volume, buySell);
    }

    AsianForward::AsianForward(AsianForward &forward)
    {
        vector<Date> tmpDatesVector = forward.getAveragingDates();
        set<Date> dates = set<Date>(tmpDatesVector.begin(), tmpDatesVector.end());
        constructorCommon(dates, 
                          forward.settlementDate, 
                          forward.strike, 
                          forward.volume, 
                          forward.buySell);
    }

    AsianForward* AsianForward::clone()
    {
        AsianForward *af = new AsianForward(*this);
        return af;
    }


    void AsianForward::validate()
    {
        ForwardContract::validate();
        AsianContract::validate();
        if (AsianContract::errorTracking->getHasErrors()) 
      {
         ForwardContract::errorTracking->populateErrorMessage(AsianContract::getAsianContractErrorMessages());
        }
        if (orderedAveragingDates.back() != getMaturityDate())
        {
            ForwardContract::errorTracking->populateErrorMessage("Maturity date does not equal the final averaging date");
        }
        if (orderedAveragingDates.back() > getSettlementDate())
        {
            ForwardContract::errorTracking->populateErrorMessage("Settlement date before the final averaging date");
        }
    }

    void AsianForward::validate(boost::shared_ptr<RatesEnvironment> re)
    {
        ForwardContract::validate(re);
        AsianContract::validate(re);
    }

    void AsianForward::setAveragingDates(set<Date> averagingDatesInput)
    {
        AsianContract::setAveragingDates(averagingDatesInput);
        setMaturityDate(orderedAveragingDates.back());
    }

    double AsianForward::getValue(boost::shared_ptr<RatesEnvironment> re)
    {
        Date valueDate = re->getAnchorDate();
        if (valueDate > getSettlementDate())
        {
            return 0;
        }
        if (!ForwardContract::isOK())
        {
         return numeric_limits<double>::quiet_NaN();
        }

        double premium = 0;
        vector<double> forwards, fixes;
        getFixingAndForwardRates(valueDate, fixes, forwards, re);
        double df = (re->getDRS())->getDiscountFactor(getSettlementDate());
        
        double averageForward = 0;
        for (size_t i = 0; i < forwards.size(); ++i) 
        {
            averageForward += forwards[i];
        }
        double averageRateSet = 0;
        for (size_t i = 0; i < fixes.size(); ++ i)
        {
            averageRateSet += fixes[i];
        }

        if (orderedAveragingDates.size() > 0) 
        {
            averageForward /= orderedAveragingDates.size();
            averageRateSet /= orderedAveragingDates.size();
        }

        if (valueDate < getMaturityDate()) 
        {
            premium = (averageForward - (strike - averageRateSet)) * df;
            return premium * volume * buySellAsDouble;
        }            
        else if ((valueDate >= maturityDate) && (valueDate <= settlementDate))
        {
            double averageFwd = 0;
            premium = (averageRateSet - strike) * df;
            return premium * volume * buySellAsDouble;            
        }
        return 0;
    }

    void AsianForward::getFixingAndForwardRates(Date valueDate, 
                                                vector<double> &fixingRates, 
                                                vector<double> &forwardRates,
                                                boost::shared_ptr<RatesEnvironment> re)
    {
        fixingRates.clear();
        forwardRates.clear();
        vector<Date> fixingDates, forwardDates;
        AsianContract::getFixingAndForwardDates(valueDate, fixingDates, forwardDates);

        for (size_t i = 0; i < fixingDates.size(); ++i) 
        {
            fixingRates.push_back(re->getFixingRS()->getRateSetOn(fixingDates[i]));
        }
        for (size_t i = 0; i < forwardDates.size(); ++i) 
        {
            forwardRates.push_back((re->getFRS())->getForward(forwardDates[i]));
        }
        
        return;
    }

    string AsianForward::toStringRates(boost::shared_ptr<RatesEnvironment> re)
    {
        stringstream descriptionStream;
        descriptionStream << SingleInstrumentContract::toStringRates(re);

        validate();
        if (ForwardContract::isOK()) 
        {
            descriptionStream << ",Fwd(Set)," << fixed << setprecision(4) 
                              << (re->getFRS())->getForward(getSettlementDate());

            vector<double> forwards, fixes;
            getFixingAndForwardRates((re->getFRS())->getAnchorDate(), fixes, forwards, re);
            double aveFwd = 0;
            for (size_t i = 0; i < forwards.size(); ++i) 
            {
                aveFwd += forwards[i];
            }
            if (forwards.size() > 0)
                aveFwd /= forwards.size();

            double aveRateSet = 0;
            for (size_t i = 0; i < fixes.size(); ++i) 
            {
                aveRateSet += fixes[i];
            }
            if (fixes.size() > 0)
            {
                aveRateSet /= fixes.size();
            }

            descriptionStream <<",Fix(Ave)," << fixed << setprecision(4) << aveRateSet;
            descriptionStream << ",Fwd(Ave)," << fixed << setprecision(4) << aveFwd;
        }
        return descriptionStream.str();        
    }

}

