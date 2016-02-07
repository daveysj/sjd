#include "SingleFactorGMB.h"


namespace sjd 
{
    /*======================================================================================
    EconomicScenarioGenerator
    =======================================================================================*/
    void EconomicScenarioGenerator::resetErrorMessages()
    {
        errorTracking->clearErrorMessages();
    }

    vector<string> EconomicScenarioGenerator::getErrorMessages()    
    {
        return errorTracking->getErrorMessages();
    }

    string EconomicScenarioGenerator::getErrorMessageAsString()    
    {
        return errorTracking->getErrorMessagesAsString();
    }

    bool EconomicScenarioGenerator::isOK()
    {
        resetErrorMessages();
        if (re == 0)
        {
            errorTracking->populateErrorMessage("Rates environment not set");
            return false;
        }
        if (!re->isOK())
        {
            errorTracking->populateErrorMessage(re->getErrorMessages());
        }
        if ((simulationDates.size() > 0) && (simulationDates.front() < anchorDate)) 
        {
            errorTracking->populateErrorMessage("Simulation dates before anchor date");
            return false;
        }
        if ((simulationDates.size() > 0) && (!re->isInRange(simulationDates.back())))
        {
            errorTracking->populateErrorMessage("Simulation dates after rate source end");
            return false;
        }
        return !errorTracking->getHasErrors();
    }

    Date EconomicScenarioGenerator::getAnchorDate()
    {
        return anchorDate;
    }

    void EconomicScenarioGenerator::setBaseRates(const boost::shared_ptr<RatesEnvironment> reInput)
    {
        re = reInput;
        anchorDate = re->getAnchorDate();
    }

    boost::shared_ptr<RatesEnvironment> EconomicScenarioGenerator::getBaseRates()
    {
        return re;
    }

    bool EconomicScenarioGenerator::setSimulationDates(set<Date> simulationDatesSet)
    {
        simulationDates = vector<Date>(simulationDatesSet.begin(), simulationDatesSet.end());
        numDates = simulationDates.size();
        return true;
    }

    /*======================================================================================
    SingleFactorEconomicScenarioGenerator
    =======================================================================================*/
    SingleFactorEconomicScenarioGenerator::SingleFactorEconomicScenarioGenerator(const boost::shared_ptr<RatesEnvironment> re)
                                                : nd(0,1), rng((boost::uint32_t) time(0)), normalVariable(rng, nd)
    {
        errorTracking = boost::shared_ptr<sjdTools::ErrorTracking>(new sjdTools::ErrorTracking("SingleFactorEconomicScenarioGenerator"));
        setBaseRates(re);
    }

    void SingleFactorEconomicScenarioGenerator::setBaseRates(const boost::shared_ptr<RatesEnvironment> re)
    {
        EconomicScenarioGenerator::setBaseRates(re);
        frsDependsOnInterestRateDifferential = false;
        if (re->hasDRS())
        {
            drs = boost::dynamic_pointer_cast<sjd::DeterministicDiscountRateSource>(re->getDRS()); 
        }
        if (re->hasForeignDRS())
        {
            foreigndrs = boost::dynamic_pointer_cast<sjd::DeterministicDiscountRateSource>(re->getForeignDRS()); 
        }
        if (re->hasVRS())
        {
            vrs = boost::dynamic_pointer_cast<sjd::DeterministicVolatilityRateSource>(re->getVRS()); 
        }
        if (re->hasFRS())
        {
            frs = boost::dynamic_pointer_cast<sjd::ForwardRateSourceStochastic>(re->getFRS()); 
            boost::shared_ptr<sjd::UsesInterestRateDifferential> tmpFRS = 
                boost::dynamic_pointer_cast<sjd::UsesInterestRateDifferential>(frs); 
            if (tmpFRS != 0)
            {
                frsDependsOnInterestRateDifferential = true;
            }
        }
        if (re->hasFixingRS())
        {
            fixingrs = boost::dynamic_pointer_cast<sjd::RollingFixingRateSource>(re->getFixingRS()); 
        }
    }

    bool SingleFactorEconomicScenarioGenerator::isOK()
    {
        if (!EconomicScenarioGenerator::isOK())
        {
            return false;
        }
        if (drs == 0)
        {
            errorTracking->populateErrorMessage(
                "Rates environement does not contain a discounting rate source that rolls deterministically");
        }
        if (foreigndrs == 0 && (re->hasForeignDRS())) // foreign DRS may not be required 
        {
            errorTracking->populateErrorMessage(
                "Rates environement does not contain a foreign discounting rate source that rolls deterministically");
        }
        if (vrs == 0)
        {
            errorTracking->populateErrorMessage(
                "Rates environement does not contain a volatility rate source that rolls deterministically");
        }
        if (frs == 0)
        {
            errorTracking->populateErrorMessage(
                "Rates environement does not contain a forward rate source that rolls stochastically");
        }
        if (frsDependsOnInterestRateDifferential && foreigndrs == 0)
        {
            errorTracking->populateErrorMessage(
                "FRS requires a foreign interest rate source but rates environment does not contain one that rolls deterministically");
        }
        if (re->hasFixingRS() && (fixingrs == 0))
        {
            errorTracking->populateErrorMessage(
                "Rates environement does not contain a fixing rate source that rolls");
        }
        return !errorTracking->getHasErrors();
    }

    bool SingleFactorEconomicScenarioGenerator::setSimulationDates(set<Date> simulationDatesSet)
    {
        EconomicScenarioGenerator::setSimulationDates(simulationDatesSet);
        if (isOK())
        {
            drs->setForwardValuationDates(simulationDatesSet); 
            drss = drs->rollForward();

            if (foreigndrs != 0)
            {
                foreigndrs->setForwardValuationDates(simulationDatesSet); 
                foreigndrss = foreigndrs->rollForward();
            }

            vrs->setForwardValuationDates(simulationDatesSet);
            vrss = vrs->rollForward();

            frs->setForwardValuationDates(simulationDatesSet);
            // fill it with something
            randomNumbers = vector<double>(numDates, 0);
            frs->setRandomVariables(randomNumbers);
            frss = frs->rollForward();

            fixingrss.clear();
            
            vector<boost::shared_ptr<RatesEnvironment>> ratesPath;
            for (size_t i = 0; i < numDates; ++i)
            {
                boost::shared_ptr<RatesEnvironment> tmpRE = boost::shared_ptr<RatesEnvironment>(
                    new RatesEnvironment(simulationDates[i]));
                tmpRE->setDRS(drss[i]);
                if (foreigndrs != 0)
                {
                    tmpRE->setForeignDRS(foreigndrss[i]);
                    if (frsDependsOnInterestRateDifferential)
                    {
                        updateFXForwardWithRolledDiscountRateSources(i);
                    }
                }
                tmpRE->setFRS(frss[i]);
                tmpRE->setVRS(vrss[i]);
                if (re->hasFixingRS()) 
                {
                    allFixingDates = fixingrs->getFixingDatesFor(simulationDates);
                    allFixingRates = vector<double>(allFixingDates.size(), 0);
                    boost::shared_ptr<RollingFixingRateSource> rolledfixingrs = 
                        fixingrs->rollForward(simulationDates[i], allFixingDates, allFixingRates);
                    fixingrss.push_back(rolledfixingrs);
                    tmpRE->setFixingRS(rolledfixingrs);
                }
                ratesPath.push_back(tmpRE);
            }
            path = boost::shared_ptr<Path>(new Path(ratesPath));
            return true;
        }
        return false;
    }

    void SingleFactorEconomicScenarioGenerator::setRandomSeed(long seedInput)
    {
        seed = seedInput;
    }

    boost::shared_ptr<Path> SingleFactorEconomicScenarioGenerator::getConfidenceInterval(double confidenceInterval)
    {
        boost::math::normal dist(0.0, 1.0);
        double epsi = quantile(dist, confidenceInterval);

        frs->setRandomVariables(vector<double>(numDates, epsi));
        if (fixingrs == 0)
        {
            for (size_t i = 0; i < numDates; ++i)
            {
                frss[i] = frs->rollForward(simulationDates[i], epsi);
                if (frsDependsOnInterestRateDifferential)
                {
                    updateFXForwardWithRolledDiscountRateSources(i);
                }
                path->getRatesEnvironment(i)->setFRS(frss[i]);
            }
        }
        else
        {
            vector<Date>::iterator endIt;
            for (size_t i = 0; i < numDates; ++i)
            {
                endIt = upper_bound(allFixingDates.begin(), allFixingDates.end(), simulationDates[i]);
                vector<Date> datesSubset = vector<Date>(allFixingDates.begin(), endIt);
                vector<double> rateSets = vector<double>(datesSubset.size());
                frss[i] = frs->rollForward(simulationDates[i], epsi, datesSubset, rateSets);
                if (frsDependsOnInterestRateDifferential)
                {
                    updateFXForwardWithRolledDiscountRateSources(i);
                }
                fixingrss[i]->updateAppedededRates(rateSets);
                path->getRatesEnvironment(i)->setFRS(frss[i]);
                path->getRatesEnvironment(i)->setFixingRS(fixingrss[i]);
            }
        }
        return path;
    }

    boost::shared_ptr<Path> SingleFactorEconomicScenarioGenerator::getRandomPath()
    {
        for (size_t i = 0; i < numDates; ++i) 
        {
            randomNumbers[i] = normalVariable();
        }
        return getPathForRandomNumber(randomNumbers);
    }

    boost::shared_ptr<Path> SingleFactorEconomicScenarioGenerator::getAntitheticPath()
    {
        for (size_t i = 0; i < numDates; ++i) 
        {
            randomNumbers[i] = -randomNumbers[i];
        }
        return getPathForRandomNumber(randomNumbers);
    }

    boost::shared_ptr<Path> SingleFactorEconomicScenarioGenerator::getPathForRandomNumber(vector<double> randomNumbers)
    {
        frs->setRandomVariables(randomNumbers);
        if (fixingrs == 0)
        {
            frss = frs->rollForward();
        }
        else
        {
            frss = frs->rollForward(allFixingDates, allFixingRates);
            for (size_t i = 0; i < fixingrss.size(); ++i)
            {
                fixingrss[i]->updateAppedededRates(allFixingRates);
            }
        }

        for (size_t i = 0; i < numDates; ++i) 
        {
            path->getRatesEnvironment(i)->setFRS(frss[i]);
        }        
        return path;
    }

    boost::shared_ptr<Path> SingleFactorEconomicScenarioGenerator::getPathForSpot(vector<double> spot)
    {
        vector<double> impliedRV = frs->getImpliedRandomNumbers(spot);
        return getPathForRandomNumber(impliedRV);
    }


    void SingleFactorEconomicScenarioGenerator::updateFXForwardWithRolledDiscountRateSources(size_t stepNumber)
    {
        boost::shared_ptr<sjd::UsesInterestRateDifferential> tmpFRS = 
            boost::dynamic_pointer_cast<sjd::UsesInterestRateDifferential>(frss[stepNumber]); 
        if (tmpFRS != 0)
        {
            tmpFRS->setRateSources(drss[stepNumber], foreigndrss[stepNumber]);
        }

    }

}