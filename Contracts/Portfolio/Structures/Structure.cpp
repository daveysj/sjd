#include "Structure.h"
#include <iomanip> 
#include <algorithm>

namespace sjd 
{

   /*======================================================================================
   Structure

    =======================================================================================*/
    Structure::Structure(boost::shared_ptr<SingleInstrumentContract> underlyingContract, 
                         boost::shared_ptr<RegCapUnderlyingFactors> regCapAssetFactors)
    {
        vector<boost::shared_ptr<SingleInstrumentContract> > underlyingContracts;
        underlyingContracts.push_back(underlyingContract);
        commonConstructor(underlyingContracts, regCapAssetFactors);
    }

    Structure::Structure(vector<boost::shared_ptr<SingleInstrumentContract> > underlyingContracts, 
                         boost::shared_ptr<RegCapUnderlyingFactors> regCapAssetFactors)
    {
        commonConstructor(underlyingContracts, regCapAssetFactors);
    }

    Structure::Structure(boost::shared_ptr<Contract> underlyingContract, 
                         boost::shared_ptr<RegCapUnderlyingFactors> regCapAssetFactors)
    {
        vector<boost::shared_ptr<SingleInstrumentContract> > underlyingContracts;

        if (boost::dynamic_pointer_cast<SingleInstrumentContract>(underlyingContract) != 0) 
        {
            underlyingContracts.push_back(boost::dynamic_pointer_cast<SingleInstrumentContract>(underlyingContract));
        }
        else if (boost::dynamic_pointer_cast<MultipleInstrumentContract>(underlyingContract) != 0) 
        {
            underlyingContracts = boost::dynamic_pointer_cast<MultipleInstrumentContract>(underlyingContract)->getUnderlyingSingleInstrumentContracts();
        }
        else
        {
            errorTracking->populateErrorMessage("Cound not convert Contract into a Structure");
        }
        commonConstructor(underlyingContracts, regCapAssetFactors);
    }


    Structure::Structure(Structure &structure)
    {
        commonConstructor(structure.contracts, structure.regCapAssetFactors);
    }

    Structure* Structure::clone()
    {
        Structure *structure = new Structure(*this);
        return structure;
    }

    void Structure::commonConstructor(vector<boost::shared_ptr<SingleInstrumentContract> > underlyingContracts, boost::shared_ptr<RegCapUnderlyingFactors> inputFactors)
    {
        errorTracking = boost::shared_ptr<sjdTools::ErrorTracking>(new sjdTools::ErrorTracking("Structure"));
        cappedExposure = false;
        cap = numeric_limits<double>::quiet_NaN(); 
        contracts = underlyingContracts;
        regCapAssetFactors = inputFactors;
        cappedExposure = isCapped(contracts, cap, hasPriceExposure);
        volume = getEffectiveVolume(contracts);
    }

    void Structure::validate()
    {
        MultipleInstrumentContract::validate();
        if (!isPiecewiseLinear(contracts)) 
        {
            errorTracking->populateErrorMessage("Structure Design Error, Payoff function is not piecewise linear");
        }
        if (volume < 1e-14 && hasPriceExposure)
        {
            errorTracking->populateErrorMessage("Structure Design Error, the structure has no volume but it does have price exposure");
        }

        if (contracts.size() > 1) 
        {
            string reasonIfNot;
            boost::shared_ptr<SingleInstrumentContract> contract1 = contracts[0];
            for (size_t i = 1; i < contracts.size(); ++i) 
            {
                boost::shared_ptr<SingleInstrumentContract> contract2 = contracts[i];
                if (!qualifyForRegulatoryCapitalOffset(contract1, contract2, reasonIfNot)) 
                {
                    errorTracking->populateErrorMessage("not all the contracts quality for regulatory capital offset, " + reasonIfNot);
                }
            }
        }        
        if (!cappedExposure) 
        {
            if (!isMaximumVolumeInUncappedWing(contracts)) 
            {
                return errorTracking->populateErrorMessage("has uncapped exposure but maximum volume is not in the (uncapped) wings");
            }
        }
    }

    void Structure::validate(boost::shared_ptr<RatesEnvironment> re)
    {
        MultipleInstrumentContract::validate(re);

        if (!isPiecewiseLinear(contracts)) 
        {
            errorTracking->populateErrorMessage("Structure Design Error, Payoff function is not piecewise linear");
        }

        if (!cappedExposure) 
        {
            if (!isMaximumVolumeInUncappedWing(contracts)) 
            {
                errorTracking->populateErrorMessage("has uncapped exposure but maximum volume is not in the (uncapped) wings");
            }
        }
    }

    Date Structure::getSettlementDate()
    {
        Date settlementDate;
        if (contracts.size() != 0) 
        {
            settlementDate = contracts[0]->getSettlementDate();
        }
        else 
        {
            settlementDate = Date(1, Jan, 1910);;
        }
        return settlementDate;
    }

    bool Structure::isExposureCapped() const
    {
        return cappedExposure;
    }

    bool Structure::getHasPriceExposure() const
    {
        return hasPriceExposure;
    }

    double Structure::getCap() const 
    {
        return cap;
    }

    double Structure::getPayoff(double forSpot)
    {
        double payoff = 0;
        for (size_t i = 0; i < contracts.size(); ++i) 
      {
            payoff += contracts[i]->getPayoff(forSpot);
      }
        return payoff;
    }

    double Structure::getMtMPlusAddOn(boost::shared_ptr<RatesEnvironment> re, double netGrossRatio)
    {
        double mtm = getValue(re);
        if (errorTracking->getHasErrors())
        {
            return numeric_limits<double>::quiet_NaN();
        }

        return getMtMPlusAddOnForGivenMtM(re, mtm, netGrossRatio);
    }

    double Structure::getMtMPlusAddOnForGivenMtM(boost::shared_ptr<RatesEnvironment> re, double mtm, double netGrossRatio)
    {
        if (re->getAnchorDate() > getSettlementDate())
        {
            return 0;
        }

        if (errorTracking->getHasErrors())
        {
            return numeric_limits<double>::quiet_NaN();
        }

        double addOn = 0;
        if (!hasPriceExposure) 
        {
            return cap;
        }
        else 
        {
            double spot = -1;
            // double volume = getEffectiveVolume(contracts);
            vector<boost::shared_ptr<SingleInstrumentContract>>::iterator it = contracts.begin();
            while ((it != contracts.end()) && (spot < 0)) 
            {
                if (isContactADerivative(*it)) 
                {
                    boost::shared_ptr<SingleDerivativeContract> derivative = boost::dynamic_pointer_cast<SingleDerivativeContract>(*it);
                    spot = re->getFRS()->getForward(re->getAnchorDate());
                }
                ++it;
            }
            if ((it == contracts.end()) && ((spot < 0) || (volume < 0))) 
            {
                return numeric_limits<double>::quiet_NaN();
            }

            Date sd = getSettlementDate();
            double tts = ((double) (sd - re->getAnchorDate())) / 365.0;
            double commodityFactor = regCapAssetFactors->getFactor(tts);
            double simpleAddOn = spot * volume * commodityFactor;
            addOn = simpleAddOn * (0.4 + 0.6 * netGrossRatio);
        }
        double mtmPlusAddOn = (mtm + addOn);

        if (cappedExposure)
        {
            mtmPlusAddOn = min(mtmPlusAddOn, cap);
        }
        return mtmPlusAddOn;
    }


    set<double> Structure::getAllStrikes(vector<boost::shared_ptr<SingleInstrumentContract> > underlyingContracts)
    {
        set<double> strikes;
        for (size_t i = 0; i < underlyingContracts.size(); ++i) 
        {
            if (isContactADerivative(underlyingContracts[i])) 
            {
                boost::shared_ptr<SingleDerivativeContract> derivative = 
                    boost::dynamic_pointer_cast<SingleDerivativeContract>(underlyingContracts[i]);
                strikes.insert(derivative->getStrike());
            }
        }
        return strikes;
    }

    vector<boost::shared_ptr<SingleInstrumentContract>> Structure::getAllContractsWithStrike(
                                        vector<boost::shared_ptr<SingleInstrumentContract> > underlyingContracts,
                                        double strike)
    {
        vector<boost::shared_ptr<SingleInstrumentContract>> contractsWithOneStrike;
        for (size_t i = 0; i < underlyingContracts.size(); ++i) 
        {
            if (isContactADerivative(underlyingContracts[i])) 
            {
                boost::shared_ptr<SingleDerivativeContract> derivative = 
                    boost::dynamic_pointer_cast<SingleDerivativeContract>(underlyingContracts[i]);
                if (abs(derivative->getStrike() - strike) < 1e-14) 
                {
                    contractsWithOneStrike.push_back(underlyingContracts[i]);
                }
            }
        }
        return contractsWithOneStrike;
    }

    double Structure::gradient(vector<boost::shared_ptr<SingleInstrumentContract> > underlyingContracts, double x1, double x2)
    {
        double y1 = 0, y2 = 0;
        for (size_t i = 0; i < underlyingContracts.size(); ++i) 
        {
            y1 += underlyingContracts[i]->getPayoff(x1);
            y2 += underlyingContracts[i]->getPayoff(x2);
        }
        return (y1 - y2) / (x1 - x2);
    }

    bool Structure::isPiecewiseLinear(vector<boost::shared_ptr<SingleInstrumentContract> > underlyingContracts)
    {
        double numericError = 1e-6;

        set<double> strikes = getAllStrikes(underlyingContracts);
        if (strikes.size() == 0)
        {
            return true;
        }
        
        double volume = getEffectiveVolume(underlyingContracts);
        if (abs(volume) < numericError) // zero volume
        {
           return true;
        }

        double lowStrike = *(strikes.begin());
        double g1 = gradient(underlyingContracts, lowStrike * .975, lowStrike * .999);
        double g2 = gradient(underlyingContracts, lowStrike * .925, lowStrike * .95);
        if (abs(g2 - g1) > numericError * volume)
        {
            return false;
        }

        if (strikes.size() > 1) 
        {
            set<double>::iterator it = strikes.begin();
            ++it;
            double x1 = lowStrike;
            double x2;
            while (it != strikes.end()) 
            {
                x2 = (*it);
                double distance = (x2 - x1) / 10; // the number 10 is arbitrary, just don't include the strikes
                g1 = gradient(underlyingContracts, x2 - distance * 3, x2 - distance * 2);
                g2 = gradient(underlyingContracts, x2 - distance * 6, x2 - distance * 5);
                if (abs(g2 - g1) > numericError * volume)
                {
                    return false;
                }
                x1 = x2;
                ++it;
            }
        }
        double highStrike = *(strikes.rbegin());
        g1 = gradient(underlyingContracts, highStrike * 1.001, highStrike * 1.025);
        g2 = gradient(underlyingContracts, highStrike * 1.05, highStrike * 1.075);

        if (abs(g2 - g1) > numericError * volume)
        {
            return false;
        }
        return true;
    }

    
    double Structure::getMaximumNominalVolume(vector<boost::shared_ptr<SingleInstrumentContract> > underlyingContracts)
    {
        double maxNominalVolume = 0;
        for (size_t i = 0; i < underlyingContracts.size(); ++i) 
        {
            if (isContactADerivative(underlyingContracts[i])) 
            {
                boost::shared_ptr<SingleDerivativeContract> derivative = boost::dynamic_pointer_cast<SingleDerivativeContract>(underlyingContracts[i]);
                maxNominalVolume = max(maxNominalVolume, derivative->getVolume());
            }
        }
        return maxNominalVolume;
    }
    
    double Structure::getEffectiveVolume(vector<boost::shared_ptr<SingleInstrumentContract> > underlyingContracts) 
    {
        set<double> strikes = getAllStrikes(underlyingContracts);
        if (strikes.size() == 0)
        {
            return numeric_limits<double>::quiet_NaN();
        }

        double lowStrike = *(strikes.begin());
        double g1 = gradient(underlyingContracts, lowStrike * .975, lowStrike * .999);
        double maxVolume = abs(g1);

        if (strikes.size() > 1) 
        {
            set<double>::iterator it = strikes.begin();
            ++it;
            double x1 = lowStrike;
            double x2;
            while (it != strikes.end()) 
            {
                x2 = (*it);
                double distance = (x2 - x1) / 10; // the number 10 is arbitrary, just don't include the strikes
                g1 = gradient(underlyingContracts, x2 - distance * 3, x2 - distance * 2);
                maxVolume = max(maxVolume, abs(g1));
                x1 = x2;
                ++it;
            }
        }
        double highStrike = *(strikes.rbegin());
        g1 = gradient(underlyingContracts, highStrike * 1.001, highStrike * 1.025);

        maxVolume = max(maxVolume, abs(g1));
        return maxVolume;

    }

    bool Structure::isMaximumVolumeInUncappedWing(vector<boost::shared_ptr<SingleInstrumentContract> > underlyingContracts)
    {
        double maxVolume = getEffectiveVolume(underlyingContracts);
        double effectiveVolumeAtLowestStrike, effectiveVolumeAthighestStrike;
        if (isUncappedOnTheLeft(underlyingContracts, effectiveVolumeAtLowestStrike))
        {
            if ((abs(effectiveVolumeAtLowestStrike) - maxVolume) > -1e-14 * maxVolume)
         {
                return true;
         }
        }
        if (isUncappedOnTheRight(underlyingContracts, effectiveVolumeAthighestStrike))
        {
            if ((effectiveVolumeAthighestStrike - maxVolume) > -1e-14 * maxVolume)
         {
                return true;
         }
        }
        return false;
    }

    bool Structure::isUncappedOnTheLeft(vector<boost::shared_ptr<SingleInstrumentContract> > underlyingContracts,
                                        double &effectiveVolumeAtLowestStrike)
    {
        set<double> strikes = getAllStrikes(underlyingContracts);
        double lowerStrike = *strikes.begin();
        effectiveVolumeAtLowestStrike = gradient(underlyingContracts, lowerStrike * .95, lowerStrike * .99);

        double maxNominalVolume = getMaximumNominalVolume(underlyingContracts);
        double ev = maxNominalVolume;
        if (ev < .001)
      {
            ev = 1;
      }
        double divisionError = 1e-10 * ev;

        if (effectiveVolumeAtLowestStrike < -divisionError)
      {
            return true;
      }
        else 
      {
            return false;
      }
    }

    bool Structure::isUncappedOnTheRight(vector<boost::shared_ptr<SingleInstrumentContract> > underlyingContracts,
                                            double &effectiveVolumeAtHighestStrike)
    {
        set<double> strikes = getAllStrikes(underlyingContracts);
        double highestStrike = *strikes.rbegin();
        effectiveVolumeAtHighestStrike = gradient(underlyingContracts, highestStrike * 1.01, highestStrike * 1.05);

        double maxNominalVolume = getMaximumNominalVolume(underlyingContracts);
        double ev = maxNominalVolume;
        if (ev < .001)
      {
            ev = 1;
      }
        double divisionError = 1e-10 * ev;

        if (effectiveVolumeAtHighestStrike > divisionError)
      {
            return true;
      }
        else 
      {
            return false;
      }
    }


    vector<boost::shared_ptr<SingleInstrumentContract> > Structure::getContractsWithUncappedExposureOnTheLeft(
                                        vector<boost::shared_ptr<SingleInstrumentContract> > underlyingContracts,
                                        double &commonStrike)
    {
        commonStrike = 0;
        vector<boost::shared_ptr<SingleInstrumentContract> > uncappedContracts;
        set<double> strikes = getAllStrikes(underlyingContracts);
        bool contractsFound = false;
        set<double>::iterator it = strikes.begin();
        while ((it != strikes.end()) && (contractsFound == false)) 
      {
            vector<boost::shared_ptr<SingleInstrumentContract> > contractsWithCommonStrike = 
                getAllContractsWithStrike(underlyingContracts, *it);
            double gradient;
            if (isUncappedOnTheLeft(contractsWithCommonStrike, gradient)) 
         {
                contractsFound = true;
                uncappedContracts = contractsWithCommonStrike;
                commonStrike = *it;
            }
            ++it;
        }
        return uncappedContracts;
    }

    vector<boost::shared_ptr<SingleInstrumentContract> > Structure::getContractsWithUncappedExposureOnTheRight(
                                        vector<boost::shared_ptr<SingleInstrumentContract> > underlyingContracts,
                                        double &commonStrike)
    {
        commonStrike = 0;
        vector<boost::shared_ptr<SingleInstrumentContract> > uncappedContracts;
        set<double> strikes = getAllStrikes(underlyingContracts);
        bool contractsFound = false;
        set<double>::reverse_iterator it = strikes.rbegin();
        while ((it != strikes.rend()) && (contractsFound == false)) 
      {
            vector<boost::shared_ptr<SingleInstrumentContract> > contractsWithCommonStrike = 
                getAllContractsWithStrike(underlyingContracts, *it);
            double gradient;
            if (isUncappedOnTheRight(contractsWithCommonStrike, gradient)) 
         {
                contractsFound = true;
                uncappedContracts = contractsWithCommonStrike;
                commonStrike = *it;
            }
            ++it;
        }
        return uncappedContracts;
    }

    bool Structure::isCapped(vector<boost::shared_ptr<SingleInstrumentContract> > underlyingContracts, double &cap, bool &hasPriceExposure)
    {
        cap = numeric_limits<double>::quiet_NaN();
        hasPriceExposure = true;

        set<double> strikes = getAllStrikes(underlyingContracts);
        if (strikes.size() == 0) 
      {
            hasPriceExposure = false;
            cap = 0;
            for (size_t i = 0; i < underlyingContracts.size(); ++i) 
         {
                cap += underlyingContracts[i]->getPayoff(1); // shouldn't matter which value you use so use any number > 0
            }
            return true;
        }

        double lowStrike = *(strikes.begin());
        double gradientAtLowStrike = gradient(underlyingContracts, lowStrike * .975, lowStrike * .999);
        
        double highStrike = *(strikes.rbegin());
        double gradientAtHighStrike = gradient(underlyingContracts, highStrike * 1.001, highStrike * 1.025);

        double maxNominalVolume = getMaximumNominalVolume(underlyingContracts);
        double ev = maxNominalVolume;
        if (ev < .001)
      {
            ev = 1;
      }
        double divisionError = 1e-10 * ev;

        if ((gradientAtLowStrike < -divisionError) || (gradientAtHighStrike > divisionError)) 
      {
            hasPriceExposure = true;
            cap = numeric_limits<double>::quiet_NaN();
            return false;
        }
        else 
      { // capped so cap will be at one of the strikes
            double maxExposure = 0;
            for (size_t i = 0; i < underlyingContracts.size(); ++i) 
         {
                maxExposure += underlyingContracts[i]->getPayoff(lowStrike);
            }
            if (strikes.size() == 1)
         {
            if (abs(gradientAtHighStrike - gradientAtLowStrike) > divisionError) 
            {
               hasPriceExposure = true;
               cap = maxExposure;
               return true;
            }
            hasPriceExposure = false;
            cap = maxExposure;
            return true;
            }
            else 
         {
                bool isFlat = true;
                set<double>::iterator it = strikes.begin();
                double s1 = *it;
                ++it;

                double tmpMaxExposure = 0;
                while (it != strikes.end()) 
            {
                    tmpMaxExposure = 0;                    
                    for (size_t i = 0; i < underlyingContracts.size(); ++i) 
               {
                        tmpMaxExposure += underlyingContracts[i]->getPayoff(*it);
                    }
                    if (abs(tmpMaxExposure - maxExposure) > divisionError)
               {
                        isFlat = false;
               }
                    maxExposure = max(tmpMaxExposure, maxExposure);                    
                    ++it;
                }

                tmpMaxExposure = 0;
                for (size_t i = 0; i < underlyingContracts.size(); ++i) 
            {
                    tmpMaxExposure += underlyingContracts[i]->getPayoff(highStrike);
                }
                if (abs(tmpMaxExposure - maxExposure) > divisionError)
            {
                    isFlat = false;
            }
                maxExposure = max(tmpMaxExposure, maxExposure);

                hasPriceExposure = !isFlat;
                cap = maxExposure;
                return true;
            }
        }
    }


    bool Structure::qualifyForRegulatoryCapitalOffset(boost::shared_ptr<SingleInstrumentContract> contract1,
                                                      boost::shared_ptr<SingleInstrumentContract> contract2,
                                                      string &reasonIfNot)
    {
        reasonIfNot = "";

        if (contract1->getSettlementDate() != contract2->getSettlementDate()) 
        {
            reasonIfNot = "Settlement Dates not equal";
            return false;
        }

        // if one or more is a cash flow, there is nothing more to check
        if ((!isContactADerivative(contract1)) || (!isContactADerivative(contract2))) 
        {
            return true;
        }
        else 
        {
            if (contract1->getMaturityDate() != contract2->getMaturityDate()) 
            {
                reasonIfNot = "Maturity dates not equal";
                return false;
            }
            boost::shared_ptr<SingleDerivativeContract> derivative1 = boost::dynamic_pointer_cast<SingleDerivativeContract>(contract1);
            boost::shared_ptr<SingleDerivativeContract> derivative2 = boost::dynamic_pointer_cast<SingleDerivativeContract>(contract2);

            if (isAnAsianDerivative(derivative1)) 
            {
                if (!isAnAsianDerivative(derivative2)) 
                {
                    reasonIfNot = "Only one of the derivatives is Asian";
                    return false;
                }
                boost::shared_ptr<AsianContract> ac1 = boost::dynamic_pointer_cast<AsianContract>(contract1);
                boost::shared_ptr<AsianContract> ac2 = boost::dynamic_pointer_cast<AsianContract>(contract2);

                vector<Date> ad1 = ac1->getAveragingDates();
                vector<Date> ad2 = ac2->getAveragingDates();
                if (ad1.size() != ad2.size()) 
                {
                    reasonIfNot = "Not all the underlying derivaitves have the same averaging dates";
                    return false;
                }
                for (size_t j = 0; j < ad1.size(); ++j) 
                {
                    if (ad1[j] != ad2[j]) 
                    {
                        reasonIfNot = "Not all the underlying derivaitves have the same averaging dates";
                        return false;
                    }
                }
            }
            else 
            { // european
                if (isAnAsianDerivative(derivative2)) 
                {
                    reasonIfNot = "One but not all of the derivatives is Asian";
                    return false;
                }
            }
            return true;
        }
    }

    bool Structure::moveContracts(vector<boost::shared_ptr<SingleInstrumentContract> > contractsToMove,
                                     vector<boost::shared_ptr<SingleInstrumentContract> > &fromContracts,
                                     vector<boost::shared_ptr<SingleInstrumentContract> > &toContracts)
    {
        bool allContractsMoved = true;
        for (size_t i = 0; i < contractsToMove.size(); ++i) 
        {
            vector<boost::shared_ptr<SingleInstrumentContract> >::iterator indexInFromContracts;
            indexInFromContracts = find(fromContracts.begin(), fromContracts.end(), contractsToMove[i]);
            if (indexInFromContracts == fromContracts.end())
            {
                allContractsMoved = false;
            }
            else 
            {
                toContracts.push_back(contractsToMove[i]);
                fromContracts.erase(indexInFromContracts);
            }
        }
        return allContractsMoved;
    }

    bool Structure::isContactADerivative(boost::shared_ptr<SingleInstrumentContract> underlyingContract)
    {
        return (boost::dynamic_pointer_cast<SingleDerivativeContract>(underlyingContract) != 0);
    }

    bool Structure::isAnAsianDerivative(boost::shared_ptr<SingleDerivativeContract> underlyingContract)
    {
        return (boost::dynamic_pointer_cast<AsianContract>(underlyingContract) != 0);
    }



    vector<boost::shared_ptr<Structure>> Structure::createStructures(vector<boost::shared_ptr<SingleInstrumentContract>> inputContracts,
                                                                           boost::shared_ptr<RegCapUnderlyingFactors> regCapAssetFactors,
                                                                           bool &successful)
    {
        successful = false;
        vector<boost::shared_ptr<Structure>> structures = vector<boost::shared_ptr<Structure>>(0);
        if (inputContracts.size() == 0) 
        {
           return structures;
        }

        if (inputContracts.size() > 1) 
        {
            boost::shared_ptr<SingleInstrumentContract> c1 = inputContracts[0];
            for (size_t i = 1; i < inputContracts.size(); ++i) 
            {
                if (!qualifyForRegulatoryCapitalOffset(c1, inputContracts[i])) 
                {
                    return structures;
                }
            }
        }

        double cap;
        bool hasExposure;
        if (isCapped(inputContracts, cap, hasExposure)) 
        {
            structures.clear();
            boost::shared_ptr<Structure> s = boost::shared_ptr<Structure>(new Structure(inputContracts, regCapAssetFactors));
            successful = !s->errorTracking->getHasErrors();
            structures.push_back(s);
            return structures;
        }
        else if (isMaximumVolumeInUncappedWing(inputContracts)) 
        {
            structures.clear();
            boost::shared_ptr<Structure> s = boost::shared_ptr<Structure>(new Structure(inputContracts, regCapAssetFactors));
            successful = !s->errorTracking->getHasErrors();
            structures.push_back(s);
            return structures;
        }
        else { // uncapped with max volume in the interior. remove convex part and create 2 strucutres
            // double maxVolume = getEffectiveVolume(inputContracts);
            vector<boost::shared_ptr<SingleInstrumentContract>> cappedContracts, uncappedContracts;
            decomposeIntoCappedAndUncapped(inputContracts, cappedContracts, uncappedContracts, successful);
            if (!successful) 
            {
                return structures;
            }


            bool opOK;
            optimiseCashflow(cappedContracts, uncappedContracts, opOK);
            if (!opOK) 
            { 
                successful = false;
                return structures;
            }

            boost::shared_ptr<Structure> cappedStructure = boost::shared_ptr<Structure>(new Structure(cappedContracts, regCapAssetFactors));            
            boost::shared_ptr<Structure> uncappedStructure = boost::shared_ptr<Structure>(new Structure(uncappedContracts, regCapAssetFactors));
            if (cappedStructure->errorTracking->getHasErrors() || uncappedStructure->errorTracking->getHasErrors()) 
            {
                successful = false;
                return structures;
            }

            structures.push_back(cappedStructure);
            structures.push_back(uncappedStructure);

        }
        successful = true;
        return structures;
    }


    void Structure::decomposeIntoCappedAndUncapped(vector<boost::shared_ptr<SingleInstrumentContract>> &allContracts,
                                                   vector<boost::shared_ptr<SingleInstrumentContract>> &cappedContracts,
                                                   vector<boost::shared_ptr<SingleInstrumentContract>> &uncappedContracts,
                                                   bool &successful)
    {
        cappedContracts.clear();
        uncappedContracts.clear();
        successful = true;
        for (size_t i = 0; i < allContracts.size(); ++i) 
        {
            cappedContracts.push_back(boost::shared_ptr<SingleInstrumentContract>(allContracts[i]->clone()));
        }
        double effectiveVolumeAtStrike;
        int infiniteLoopBreaker = cappedContracts.size() + 1;
        int loopCounter = 0;
        while (isUncappedOnTheLeft(cappedContracts, effectiveVolumeAtStrike) && (loopCounter < infiniteLoopBreaker) )
        {
            ++loopCounter;
            double commonStrike;
            vector<boost::shared_ptr<SingleInstrumentContract> > uncappedContractsWithOneStrike = 
                getContractsWithUncappedExposureOnTheLeft(cappedContracts, commonStrike);

            double gradientOfUncappedContracts = gradient(uncappedContractsWithOneStrike, commonStrike * .95, commonStrike * .99);
            if (abs(effectiveVolumeAtStrike) - abs(gradientOfUncappedContracts) > -1e-10) 
            {
                // remove all from capped and place into uncapped contracts
                moveContracts(uncappedContractsWithOneStrike, cappedContracts, uncappedContracts);
            }
            else 
            {
                // clone the contracts, scale down the volume and split between capped and uncapped
                for (size_t i = 0; i < uncappedContractsWithOneStrike.size(); ++i) {
                    boost::shared_ptr<SingleInstrumentContract> contractToMove = boost::shared_ptr<SingleInstrumentContract>(uncappedContractsWithOneStrike[i]->clone());                        
                    double scaledVolume = contractToMove->getVolume() * effectiveVolumeAtStrike / gradientOfUncappedContracts;
                    contractToMove->setVolume(scaledVolume);
                    uncappedContracts.push_back(contractToMove);
                    double residualvolume = uncappedContractsWithOneStrike[i]->getVolume() - scaledVolume;
                    uncappedContractsWithOneStrike[i]->setVolume(residualvolume);
                }
            }
        }
        if (loopCounter == infiniteLoopBreaker) 
        {
            successful = false;
            return;
        }
        infiniteLoopBreaker = cappedContracts.size() + 1;
        loopCounter = 0;
        while (isUncappedOnTheRight(cappedContracts, effectiveVolumeAtStrike) && (loopCounter < infiniteLoopBreaker) )
        {
            ++loopCounter;
            double commonStrike;
            vector<boost::shared_ptr<SingleInstrumentContract> > uncappedContractsWithOneStrike = 
                getContractsWithUncappedExposureOnTheRight(cappedContracts, commonStrike);

            double gradientOfUncappedContracts = gradient(uncappedContractsWithOneStrike, commonStrike * 1.01, commonStrike * 1.05);
            if (effectiveVolumeAtStrike - gradientOfUncappedContracts > -1e-10) 
            {
                // remove all from capped and place into uncapped contracts
                moveContracts(uncappedContractsWithOneStrike, cappedContracts, uncappedContracts);
            }
            else 
            {
                // clone the contracts, scale down the volume and split between capped and uncapped
                for (size_t i = 0; i < uncappedContractsWithOneStrike.size(); ++i) 
                {
                    boost::shared_ptr<SingleInstrumentContract> contractToMove = boost::shared_ptr<SingleInstrumentContract>(uncappedContractsWithOneStrike[i]->clone());                        
                    double scaledVolume = contractToMove->getVolume() * effectiveVolumeAtStrike / gradientOfUncappedContracts;
                    contractToMove->setVolume(scaledVolume);
                    uncappedContracts.push_back(contractToMove);
                    double residualvolume = uncappedContractsWithOneStrike[i]->getVolume() - scaledVolume;
                    uncappedContractsWithOneStrike[i]->setVolume(residualvolume);
                }
            }
        }
        if (loopCounter == infiniteLoopBreaker) 
        {
            successful = false;
            return;
        }
        return;
    }

    void Structure::optimiseCashflow(vector<boost::shared_ptr<SingleInstrumentContract>> &cappedContracts,
                                     vector<boost::shared_ptr<SingleInstrumentContract>> &unCappedContracts,
                                     bool &successful)
     {
        if ((cappedContracts.size() ==0) && (unCappedContracts.size() == 0)) 
        {
            successful = false;
            return;
        }

        Date settlement;
        if (cappedContracts.size() > 0)
        {
            settlement = cappedContracts[0]->getSettlementDate();
        }
        else
        {
            settlement = unCappedContracts[0]->getSettlementDate();
        }

        for (size_t i = 0; i < cappedContracts.size(); ++i) 
        {
            if ((cappedContracts[i]->getPremiumAmount() > 1e-14) && (cappedContracts[i]->getBuySell() == BUY))
            {
                double payable = cappedContracts[i]->getPremiumAmount() * cappedContracts[i]->getVolume();
                cappedContracts[i]->setPremiumAmount(0);
                Date premiumDate = cappedContracts[i]->getPremiumPaymentDate();
                boost::shared_ptr<FixedCashflow> cashflowPayable = 
                    boost::shared_ptr<FixedCashflow>(new FixedCashflow(premiumDate, payable, SELL));
                unCappedContracts.push_back(cashflowPayable);
            }
        }
        for (size_t i = 0; i < unCappedContracts.size(); ++i) 
        {
            if ((unCappedContracts[i]->getPremiumAmount() > 1e-14) && (unCappedContracts[i]->getBuySell() == SELL))
            {
                double receivable = unCappedContracts[i]->getPremiumAmount() * unCappedContracts[i]->getVolume();
                unCappedContracts[i]->setPremiumAmount(0);
                Date premiumDate = unCappedContracts[i]->getPremiumPaymentDate();
                boost::shared_ptr<FixedCashflow> cashflowReceivable = 
                    boost::shared_ptr<FixedCashflow>(new FixedCashflow(premiumDate, receivable, SELL));
                cappedContracts.push_back(cashflowReceivable);
            }           
        }
        successful = true;
    }

    string Structure::toString(boost::shared_ptr<RatesEnvironment> re)
    {
        validate(re);
        stringstream description;
        if (!isOK())
        {
            description << "Structure has errors,";
            vector<string> errorMessages = getErrorMessages();
            for (size_t i = 0; i < errorMessages.size(); ++i) 
            {
                description << errorMessages[i] << ",";
            }
            return description.str();
        }
        description << "Structure MtM," << fixed << setprecision(4) << getValue(re);
        if (!hasPriceExposure)
        {
            description << ",No Price Exposure," << fixed << setprecision(2) << cap;        
        }
        else if (cappedExposure) 
        {
            description << ",Capped at," << fixed << setprecision(2) << cap;        
        }
        else
        {
            description << ",Uncapped";
        }
        description << ",Volume," << fixed << setprecision(2) <<getEffectiveVolume(contracts) << "\n";
        description << toStringRates(re);
        return description.str() ;
    }
}