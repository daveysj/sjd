
#include "AsianOptionContract.h"


namespace sjd {

   /*======================================================================================
   GenericAsianOption

    =======================================================================================*/
    void AsianOption::constructorCommon(set<Date> averagingDatesInput, 
                                        Date optionSettlement, 
                                        double X, 
                                        PutCall pc, 
                                        double N,
                                        Date pDate, 
                                        double pAmount, 
                                        BuySell bs)
    {
      SingleOptionContract::errorTracking = boost::shared_ptr<sjdTools::ErrorTracking>(new sjdTools::ErrorTracking("AsianOption"));
      AsianContract::errorTracking = boost::shared_ptr<sjdTools::ErrorTracking>(new sjdTools::ErrorTracking("AsianOption"));
        setAveragingDates(averagingDatesInput);
        setSettlementDate(optionSettlement);
        setPremiumPaymentDate(pDate);
        setPremiumAmount(pAmount);
        setStrike(X);
        setVolume(N);
        setBuySell(bs);
        setType(pc);
    }

    AsianOption::AsianOption(set<Date> averagingDates, 
                             Date optionSettlement, 
                             double strike, 
                             PutCall type, 
                             double volume,
                             Date premiumPaymentDate, 
                             double premiumAmount, 
                             BuySell buySell)
    {
        constructorCommon(averagingDates, 
                          optionSettlement, 
                          strike, 
                          type, 
                          volume, 
                          premiumPaymentDate, 
                          premiumAmount, 
                          buySell);
    }

    AsianOption::AsianOption(Calendar calendar, 
                             Date startOfAveraging, 
                             Date endOfAveraging, 
                             Date optionSettlement, 
                             double strike, 
                             PutCall type, 
                             double volume,
                             Date premiumPaymentDate, 
                             double premiumAmount, 
                             BuySell buySellInput) 
    {
        set<Date> averagingDates = getAllBusinessDaysBetween(startOfAveraging, endOfAveraging, calendar);
        constructorCommon(averagingDates, 
                          optionSettlement, 
                          strike, 
                          type, 
                          volume, 
                          premiumPaymentDate, 
                          premiumAmount, 
                          buySellInput);
    }
        
    AsianOption::AsianOption(AsianOption &optionInput)
    {
        vector<Date> tmpDatesVector = optionInput.getAveragingDates();
        set<Date> dates = set<Date>(tmpDatesVector.begin(), tmpDatesVector.end());
        constructorCommon(dates, 
                          optionInput.getSettlementDate(), 
                          optionInput.getStrike(), 
                          optionInput.getType(), 
                          optionInput.getVolume(), 
                          optionInput.getPremiumPaymentDate(), 
                          optionInput.getPremiumAmount(), 
                          optionInput.getBuySell());
    }

    AsianOption* AsianOption::clone()
    {
        AsianOption *ao = new AsianOption(*this);
        return ao;
    }

    void AsianOption::setAveragingDates(set<Date> averagingDatesInput)
    {
        AsianContract::setAveragingDates(averagingDatesInput);
        setMaturityDate(orderedAveragingDates.back());
    }
            
    void AsianOption::setType(PutCall pc)
    {
        SingleOptionContract::setType(pc);
        if (pc == PUT) 
        {
            option = boost::shared_ptr<AverageRateOption>(
                new AverageRatePut(vector<double>(1,0), vector<double>(1,0), 0, vector<double>(1,0), 0));
        }
        else 
        {
            option = boost::shared_ptr<AverageRateOption>(
                new AverageRateCall(vector<double>(1,0), vector<double>(1,0), 0, vector<double>(1,0), 0));
        }
    }

    void AsianOption::validate()
    {
        SingleOptionContract::validate();
        AsianContract::validate();
        if (AsianContract::errorTracking->getHasErrors()) 
      {
         SingleOptionContract::errorTracking->populateErrorMessage(AsianContract::getAsianContractErrorMessages());
        }
        if (orderedAveragingDates.back() != getMaturityDate())
        {
            SingleOptionContract::errorTracking->populateErrorMessage("Maturity date does not equal the final averaging date");
        }
        if (orderedAveragingDates.back() > getSettlementDate())
        {
            SingleOptionContract::errorTracking->populateErrorMessage("Settlement date before the final averaging date");
        }
    }

    void AsianOption::validate(boost::shared_ptr<RatesEnvironment> re)
    {
        SingleOptionContract::validate(re);
        AsianContract::validate(re);
        if (AsianContract::errorTracking->getHasErrors()) 
        {
         SingleOptionContract::errorTracking->populateErrorMessage(AsianContract::getAsianContractErrorMessages());
        }
        if (!(re->getFRS())->extendsTo(getMaturityDate())) 
        {
            SingleOptionContract::errorTracking->populateErrorMessage("Contract settlement not in forward rate source range");
        }
        if (!(re->getVRS())->extendsTo(getSettlementDate())) 
        {
            SingleOptionContract::errorTracking->populateErrorMessage("Contract settlement not in volatility rate source range");
        }
    }

    double AsianOption::getOptionValue(boost::shared_ptr<RatesEnvironment> re)
    {
        Date onDate = re->getAnchorDate();
        if (onDate > getSettlementDate())
        {
            return 0;
        }
        vector<double> forwards, fixes, sds, vols;
        getRates(onDate, fixes, forwards, sds, vols, re);

        double premium = 0;

        if (sds.size() != forwards.size())
        {
            return numeric_limits<double>::quiet_NaN();
        }

        double df = (re->getDRS())->getDiscountFactor(getSettlementDate());
        
        if (onDate < maturityDate) 
        {
            option->setParameters(forwards, fixes, strike, sds, df);    
            premium = option->getPremium();
        }            
        else if ((onDate >= maturityDate) && (onDate <= settlementDate))
        {
            double averageFwd = 0;
            option->setParameters(forwards, fixes, strike, sds, df);    
            if (fixes.size() == 0)
            {
                return 0;
            }
            for (size_t i = 0; i < fixes.size(); ++i) 
            {
                averageFwd += fixes[i];
            }
            averageFwd /= fixes.size();
            premium = option->getPremiumAfterMaturity(averageFwd, df);
        }
        return premium * volume;
    }
    
    void AsianOption::getRates(Date valueDate, 
                               vector<double> &fixes, 
                               vector<double> &forwards,
                               vector<double> &sd, 
                               vector<double> &vol,
                               boost::shared_ptr<RatesEnvironment> re)
    {
        vector<Date> fixingDates, forwardDates;
        AsianContract::getFixingAndForwardDates(valueDate, fixingDates, forwardDates);
        
        fixes = (re->getFixingRS())->getRateSetOn(fixingDates);
        forwards = (re->getFRS())->getForward(forwardDates);

        sd = vector<double>(forwardDates.size());
        vol = vector<double>(forwardDates.size());

        (re->getVRS())->getVolAndSD(forwardDates, strike, forwards, vol, sd);


        //if (valueDate < getMaturityDate()) 
        //{   
        //    for (size_t i = 0; i < forwardDates.size(); ++i) 
        //    {
        //        (re->getVRS())->getVolAndSD(forwardDates[i], getStrike(), forwards[i], vol[i], sd[i]);
        //    }
        //}               
   }

   string AsianOption::toStringRates(boost::shared_ptr<RatesEnvironment> re)
    {
        stringstream ratesDescriptionStream;
        ratesDescriptionStream << SingleInstrumentContract::toStringRates(re);
        validate();
        if (SingleOptionContract::isOK())
        {
            vector<double> forwards, fixes;
            vector<double> sds, vols;
            getRates((re->getFRS())->getAnchorDate(), fixes, forwards, sds, vols, re);
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

            ratesDescriptionStream <<",Fix(Ave)," << fixed << setprecision(4) << aveRateSet;
            ratesDescriptionStream << ",Fwd(Ave)," << fixed << setprecision(4) << aveFwd;

            double averageSD = 0, averageVol = 0;
            for (size_t i = 0; i < sds.size(); ++i) 
            {
                averageSD += sds[i] * sds[i];
                averageVol += vols[i];
            }
            if (sds.size() > 0) 
            {
                averageSD /= sds.size();
                averageVol /= sds.size();
            }
            averageSD = sqrt(averageSD);

            ratesDescriptionStream << ",Vol(Ave)," << fixed << setprecision(6) << averageVol
                                   << ",SD(Ave)," << fixed << setprecision(6) << averageSD;

        }
        return ratesDescriptionStream.str();
    }

}
