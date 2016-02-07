
#include "Portfolio.h"


namespace sjd 
{

    /*======================================================================================
    Portfolio
    
    =======================================================================================*/
    void Portfolio::setInputVariables()
    {
        errorTracking = boost::shared_ptr<sjdTools::ErrorTracking>(new sjdTools::ErrorTracking("Portfolio"));
    }

    Portfolio::Portfolio(vector<boost::shared_ptr<SingleInstrumentContract>> inputs)
    {
        setInputVariables();
        contracts = inputs;
        hasMargin = false;
    }

    Portfolio::Portfolio(vector<boost::shared_ptr<SingleInstrumentContract>> inputs,
                         boost::shared_ptr<Margin> marginInputs)
    {
        setInputVariables();
        contracts = inputs;
        hasMargin = true;
        margin = marginInputs;
    }

    Portfolio::Portfolio(vector<boost::shared_ptr<Contract>> inputs)
    {
        setInputVariables();
        vector<boost::shared_ptr<SingleInstrumentContract>> inputsAsSingleInstruments = 
            getSingleInstrumentContracts(inputs);

        if (errorTracking->getHasErrors()) 
        {
            contracts = vector<boost::shared_ptr<SingleInstrumentContract>>(0);
            errorTracking->populateErrorMessage("Cannot extract single instruments from input contracts");
        }
        else 
        {
            contracts = inputsAsSingleInstruments;
        }
        hasMargin = false;
    }

    Portfolio::Portfolio(vector<boost::shared_ptr<Contract>> inputs,
                         boost::shared_ptr<Margin> marginInput)
    {
        setInputVariables();
        vector<boost::shared_ptr<SingleInstrumentContract>> inputsAsSingleInstruments = 
            getSingleInstrumentContracts(inputs);

        if (errorTracking->getHasErrors()) 
        {
            contracts = vector<boost::shared_ptr<SingleInstrumentContract>>(0);
            errorTracking->populateErrorMessage("Cannot extract single instruments from input contracts");
        }
        else 
        {
            contracts = inputsAsSingleInstruments;
        }
        hasMargin = true;
        margin = marginInput;
    }

    Portfolio::Portfolio(Portfolio &portfolio)
    {
        setInputVariables();
        contracts = portfolio.contracts;

        if (portfolio.hasMargin) 
        {
            hasMargin = true;
            margin = portfolio.margin;
        }
        else 
        {
            hasMargin = false;
        }
    }

    Portfolio* Portfolio::clone()
    {
        Portfolio *portfolio = new Portfolio(*this);
        return portfolio;
    }

    void Portfolio::validate() 
   {
        MultipleInstrumentContract::validate();
    }

    void Portfolio::validate(boost::shared_ptr<RatesEnvironment> re) 
    {
        return MultipleInstrumentContract::validate(re);
    }

    bool Portfolio::isMargined()
    {
        return hasMargin;
    }

    boost::shared_ptr<Margin> Portfolio::getMargin()
    {
        return margin;
    }

    void Portfolio::setMarginBalance(double balance) 
    {
        if (hasMargin) 
        {
            margin->setMarginBalance(balance);
        }
    }

    double Portfolio::getMarginBalance() 
    {
        if (margin) 
        {
            return margin->getMarginBalance();
        }
        return 0;
    }

    double Portfolio::getMarginCall(boost::shared_ptr<RatesEnvironment> re) 
    {
        if (margin) 
        {
            double mtm = getValue(re);
            return margin->getMarginCall(mtm);
        }
        return 0;
    }
        
    void Portfolio::updateMarginBalanceWithCall(double marginCall)
    {
        if (margin)
        {
            margin->setMarginBalance(margin->getMarginBalance() + marginCall);
        }
    }

    vector<boost::shared_ptr<SingleInstrumentContract>> Portfolio::getUnderlyingSingleInstrumentContracts()
    {
        return contracts;
    } 

   /*======================================================================================
    PortfolioWithRegulatoryCapital
   
    Interface for Portfolios that will have regulatory capital
    =======================================================================================*/
    void PortfolioWithRegulatoryCapital::setRegulatoryCapitalInputs(boost::shared_ptr<RegCapUnderlyingFactors> regCapAssetFactorsInput,
                                                                    boost::shared_ptr<RegCapCreditParameters> regCapCreditParametersInput)
    {
        regCapAssetFactors = regCapAssetFactorsInput;
        regCapCreditParameters = regCapCreditParametersInput;
    }

    boost::shared_ptr<RegCapUnderlyingFactors> PortfolioWithRegulatoryCapital::getRegCapAssetFactors()
    {
        return regCapAssetFactors;
    }

    boost::shared_ptr<RegCapCreditParameters> PortfolioWithRegulatoryCapital::getRegCapCreditParameters()
    {
        return regCapCreditParameters;
    }

    /*======================================================================================
    PortfolioCEM - Current Exposure Method
    
    =======================================================================================*/

    void PortfolioCEM::construct(vector<boost::shared_ptr<Contract>> inputs,
                                boost::shared_ptr<RegCapUnderlyingFactors> regCapAssetFactorsInput,
                                boost::shared_ptr<RegCapCreditParameters> regCapCreditParametersInput,
                                bool optimise,
                                boost::shared_ptr<Margin> marginInput)
    {
        setInputVariables();
        setRegulatoryCapitalInputs(regCapAssetFactorsInput, regCapCreditParametersInput);
        hasMargin = false;
        if (marginInput != 0)
        {
            hasMargin = true;
            margin = marginInput;
        }

        vector<boost::shared_ptr<SingleInstrumentContract>> inputsAsSingleInstruments = 
            getSingleInstrumentContracts(inputs);

        if (errorTracking->getHasErrors()) 
        {
            contracts = vector<boost::shared_ptr<SingleInstrumentContract>>(0);
            structures = vector<boost::shared_ptr<Structure>>(0); 
            errorTracking->populateErrorMessage("Cannot extract single instruments from input contracts");
            return;
        }

        if (optimise) 
        {
            bool successful;
            structures = optimiseStructuresFromContracts(inputsAsSingleInstruments, regCapAssetFactors, successful, structureConversionErrorMessage);
            structureConversionError = !successful;
            if (!successful) 
            {
                errorTracking->populateErrorMessage("" + structureConversionErrorMessage);
            }
        }
        else 
        {
            for (size_t i = 0; i < inputs.size(); ++i) 
            {
                boost::shared_ptr<Structure> s = boost::shared_ptr<Structure>(new Structure(inputs[i], regCapAssetFactorsInput));
                if (!s->isOK()) 
                {
                    errorTracking->populateErrorMessage("Cannot extract structures from inputs");
                    structureConversionError = true;
                }
                else 
                {
                    structures.push_back(s);
                }
            }
        }
        if (errorTracking->getHasErrors()) 
        {
            contracts = vector<boost::shared_ptr<SingleInstrumentContract>>(0);
            structures = vector<boost::shared_ptr<Structure>>(0); 
        }
        else 
        {
            contracts = inputsAsSingleInstruments;
        }
    }

    void PortfolioCEM::setInputVariables()
    {
      errorTracking = boost::shared_ptr<sjdTools::ErrorTracking>(new sjdTools::ErrorTracking("PortfolioCEM"));

        Portfolio::setInputVariables();
        structureConversionError = false;
        structureConversionErrorMessage = "Structures not yet created";
        if (regCapAssetFactors == 0)
        {
            regCapAssetFactors = generateRegCapFactors("commodity");
        }
        if (regCapCreditParameters == 0)
        {
            regCapCreditParameters = 
                boost::shared_ptr<RegCapCreditParameters>(new RegCapCreditParameters(0,0));
        }
    }

    void PortfolioCEM::validate() 
    {
        if (structureConversionError)
        {
            errorTracking->populateErrorMessage("" + structureConversionErrorMessage);
            return;
        }

        for (size_t i = 0; i < structures.size(); ++i) 
        {
            structures[i]->validate();
            if (!structures[i]->isOK())
            {
                errorTracking->populateErrorMessage(structures[i]->getErrorMessages());
            }
        }
        PortfolioWithRegulatoryCapital::validate();
    }

    void PortfolioCEM::validate(boost::shared_ptr<RatesEnvironment> re) 
    {
        if (structureConversionError) 
        {
            errorTracking->populateErrorMessage("" + structureConversionErrorMessage);
            return;
        }

        for (size_t i = 0; i < structures.size(); ++i) 
        {
            structures[i]->validate(re);
            if (!structures[i]->isOK())
            {
                return errorTracking->populateErrorMessage(structures[i]->getErrorMessages());
            }
        }
        PortfolioWithRegulatoryCapital::validate(re);
    }

    vector<boost::shared_ptr<Structure>> PortfolioCEM::optimiseStructuresFromContracts(vector<boost::shared_ptr<SingleInstrumentContract>> contractsInput,
                                                                                       boost::shared_ptr<RegCapUnderlyingFactors> regCapAssetFactors,
                                                                                       bool &sucessfullportfolio,
                                                                                       string &errorMessage)    
    {        
        sucessfullportfolio = true;
        errorMessage = "No error converting structures to portfolio";
        if (contractsInput.size() == 0) 
        {
            sucessfullportfolio = false;
            errorMessage = "No contracts to convert into structures";            
            return vector<boost::shared_ptr<Structure>>(0);
        }

        list<boost::shared_ptr<SingleInstrumentContract>> tmpContracts;
        for (size_t i = 0; i < contractsInput.size(); ++i)
        {
            tmpContracts.push_back(boost::shared_ptr<SingleInstrumentContract>(contractsInput[i]->clone()));
        }

        vector<boost::shared_ptr<SingleInstrumentContract>> potentialContractsForOneStructure;
        vector<boost::shared_ptr<Structure>> allStructures;        
        while (tmpContracts.size() > 0) 
        {
            // Step 1 Collect contracts that qualify for full / partial reg cap offset
            potentialContractsForOneStructure.clear();
            potentialContractsForOneStructure.push_back(tmpContracts.front());
            tmpContracts.erase(tmpContracts.begin());
            list<boost::shared_ptr<SingleInstrumentContract>>::iterator it = tmpContracts.begin();
            while (it != tmpContracts.end()) 
            { 
                if (Structure::qualifyForRegulatoryCapitalOffset(*it, potentialContractsForOneStructure.front()))
                {
                    potentialContractsForOneStructure.push_back(*it);
                    it = tmpContracts.erase(it);
                }
                else 
                {
                    ++it;
                }
            }

            // Now form the grouped contracts into structures
            bool successful;
            vector<boost::shared_ptr<Structure>> structuresForOneSettlement = 
                Structure::createStructures(potentialContractsForOneStructure, regCapAssetFactors, successful);
            if (!successful) 
            {
                sucessfullportfolio = false;
                stringstream descriptionStream;
                descriptionStream << "Unable to convert single instruments for settlement on " 
                    << QuantLib::io::iso_date(potentialContractsForOneStructure[0]->getSettlementDate()) << " into a single structure";
                errorMessage = descriptionStream.str();
            }
            for (size_t i = 0; i < structuresForOneSettlement.size(); ++i) 
            {
                allStructures.push_back(structuresForOneSettlement[i]);
            }            
        }            
        return allStructures;
    }

    double PortfolioCEM::getNGR(boost::shared_ptr<RatesEnvironment> re, double &mtm)
    {
        if (errorTracking->getHasErrors()) 
        {
            numeric_limits<double>::quiet_NaN();
        }
        if (regCapAssetFactors == NULL)
        {
            return 0;
        }
        double netValue = 0, grossValue = 0, ngr = 0;
        for (size_t i = 0; i < structures.size(); ++i) 
        {
            double value = structures[i]->getValue(re);
            netValue += value;
            grossValue += max(0.0, value);
        }
        mtm = netValue;
        if (netValue < 0)
        {
            ngr = 0;
        }
        else if (grossValue > 1e-14) // If the derivative matures with zero value
        {
            ngr = netValue / grossValue; 
        }
        else 
        {
            ngr = 0;
        }
        return ngr;
    }

    string PortfolioCEM::toString(boost::shared_ptr<RatesEnvironment> re) 
    {
        validate(re);
        stringstream description;
        if (!isOK())
        {
            description << "Portfolio has errors,";
            vector<string> errorMessages = getErrorMessages();
            for (size_t i = 0; i < errorMessages.size(); ++i) 
            {
                description << errorMessages[i] << ",";
            }
            return description.str();
        }
        double mtm = 0, ngr = 0, regCap = 0;
        ngr = getNGR(re, mtm);
        regCap = getRegulatoryCapital(re);
        description << "Portfolio MtM," << fixed << setprecision(4) << mtm << 
                        ",NGR," << fixed << setprecision(8) << ngr << 
                        ",RegCap," << fixed << setprecision(4) << regCap << "\n";

        for (size_t i = 0; i < structures.size(); ++i)
        {
            description << structures[i]->toString(re);
        }
        return description.str();        
    }


   /*======================================================================================
   Portfolio - Basel II, Current Exposure Method
    
    A portfolio consists of one or more structures
    =======================================================================================*/
    void PortfolioB2CEM::setInputVariables()
    {
        PortfolioCEM::setInputVariables();
        errorTracking = boost::shared_ptr<sjdTools::ErrorTracking>(new sjdTools::ErrorTracking("PortfolioB2CEM"));
    }


    PortfolioB2CEM::PortfolioB2CEM(vector<boost::shared_ptr<Contract>> inputs,
                                   boost::shared_ptr<RegCapUnderlyingFactors> regCapAssetFactorsInput,
                                   boost::shared_ptr<RegCapCreditParameters> regCapCreditParametersInput,
                                   bool optimise)

    {
        PortfolioCEM::construct(inputs, regCapAssetFactorsInput, regCapCreditParametersInput, optimise, boost::shared_ptr<Margin>());
    }

    PortfolioB2CEM::PortfolioB2CEM(vector<boost::shared_ptr<Contract>> inputs,
                                   boost::shared_ptr<RegCapUnderlyingFactors> regCapAssetFactorsInput,
                                   boost::shared_ptr<RegCapCreditParameters> regCapCreditParametersInput,
                                   bool optimise,
                                   boost::shared_ptr<Margin> margin)
    {
        PortfolioCEM::construct(inputs, regCapAssetFactorsInput, regCapCreditParametersInput, optimise, margin);
    }


    PortfolioB2CEM::PortfolioB2CEM(PortfolioB2CEM &portfolio)
    {
        setInputVariables();
                
        vector<boost::shared_ptr<SingleInstrumentContract>> tmpContracts = portfolio.getUnderlyingSingleInstrumentContracts();
        for (size_t i = 0; i < tmpContracts.size(); ++i) 
        {
            contracts.push_back(boost::shared_ptr<SingleInstrumentContract>(tmpContracts[i]->clone()));
        }
        vector<boost::shared_ptr<Structure>> tmpStructures = portfolio.structures;
        for (size_t i = 0; i < tmpStructures.size(); ++i) 
        {
            structures.push_back(boost::shared_ptr<Structure>(tmpStructures[i]->clone()));
        }
        
        margin = portfolio.margin;
        hasMargin = portfolio.hasMargin;

        regCapAssetFactors = portfolio.regCapAssetFactors;
        regCapCreditParameters = portfolio.regCapCreditParameters;
    }

    PortfolioB2CEM* PortfolioB2CEM::clone()
    {
        PortfolioB2CEM *portfolio = new PortfolioB2CEM(*this);
        return portfolio;
    }

    double PortfolioB2CEM::getRegulatoryCapital(boost::shared_ptr<RatesEnvironment> re)
    {
        double mtm;
        double ngr = getNGR(re, mtm);

        double regCap = 0;
        for (size_t i = 0; i < structures.size(); ++i) 
        {
            regCap += structures[i]->getMtMPlusAddOn(re, ngr);
        }
        if (hasMargin) 
        {
            regCap = regCap - mtm + max(0.0, mtm - margin->getMarginBalance());
        }
        else 
        {
            regCap = regCap - mtm + max(0.0, mtm);
        }
        regCap *= regCapCreditParameters->getCapitalCharge() * regCapCreditParameters->getCounterpartyWeighting();
        regCap = max(0.0, regCap);
        return regCap;
    }

   /*======================================================================================
   Portfolio - Basel III, Current Exposure Method
    
    A portfolio consists of one or more structures
    =======================================================================================*
    B3CreditRating::B3CreditRating(string ratingInput)
    {        
        boost::to_lower(ratingInput);
        hasErrors = false;
        if (ratingInput.compare("aaa") == 0) 
        {
            rating = 0.007;
        }
        else if (ratingInput.compare("aa") == 0) 
        {
            rating = 0.007;
        }
        else if (ratingInput.compare("a") == 0) 
        {
            rating = 0.008;
        }
        else if (ratingInput.compare("bbb") == 0) 
        {
            rating = 0.01;
        }
        else if (ratingInput.compare("bb") == 0) 
        {
            rating = 0.02;
        }
        else if (ratingInput.compare("b") == 0) 
        {
            rating = 0.03;
        }
        else if (ratingInput.compare("c") == 0) 
        {
            rating = 0.1;
        }
        else 
        {
            rating = numeric_limits<double>::quiet_NaN();
            hasErrors = true;
        }
    }

    bool B3CreditRating::isOK()
    {
        return !hasErrors;
    }

    double B3CreditRating::getRating()
    {
        return rating;
    }

    /*======================================================================================
   Portfolio - Basel III, Current Exposure Method
    
    =======================================================================================*
    void PortfolioB3CEM::setInputVariables()
    {
        PortfolioCEM::setInputVariables();
      errorTracking = boost::shared_ptr<sjdTools::ErrorTracking>(new sjdTools::ErrorTracking("PortfolioB3CEM"));
    }


    PortfolioB3CEM::PortfolioB3CEM(vector<boost::shared_ptr<Contract>> inputs,
                                   boost::shared_ptr<RegCapUnderlyingFactors> regCapAssetFactorsInput,
                                   boost::shared_ptr<RegCapCreditParameters> regCapCreditParametersInput,
                                   boost::shared_ptr<B3CreditRating> creditGradeInput,
                                   bool optimise) : creditRating(creditGradeInput)

    {
        PortfolioCEM::construct(inputs, regCapAssetFactorsInput, regCapCreditParametersInput, optimise);
    }

    PortfolioB3CEM::PortfolioB3CEM(PortfolioB3CEM &portfolio) : creditRating(portfolio.creditRating)
    {
        setInputVariables();
                
        vector<boost::shared_ptr<SingleInstrumentContract>> tmpContracts = portfolio.getUnderlyingSingleInstrumentContracts();
        for (size_t i = 0; i < tmpContracts.size(); ++i) 
        {
            contracts.push_back(boost::shared_ptr<SingleInstrumentContract>(tmpContracts[i]->clone()));
        }
        vector<boost::shared_ptr<Structure>> tmpStructures = portfolio.structures;
        for (size_t i = 0; i < tmpStructures.size(); ++i) 
        {
            structures.push_back(boost::shared_ptr<Structure>(tmpStructures[i]->clone()));
        }
        
        setMarginParameters(portfolio.margin);
        setPathGenerator(portfolio.pathGenerator);

        if (portfolio.orderedForwardValuationDates.size() > 0) 
        {
            set<Date> fwdDates = set<Date>(portfolio.orderedForwardValuationDates.begin(), portfolio.orderedForwardValuationDates.end());
            setForwardValuationDates(fwdDates);
        }
        regCapAssetFactors = portfolio.regCapAssetFactors;
        regCapCreditParameters = portfolio.regCapCreditParameters;
    }

    PortfolioB3CEM* PortfolioB3CEM::clone()
    {
        PortfolioB3CEM *portfolio = new PortfolioB3CEM(*this);
        return portfolio;
    }

    void PortfolioB3CEM::setMarginParameters(boost::shared_ptr<Margin> margin)
    {
        boost::shared_ptr<Calendar> calendar = boost::shared_ptr<Calendar>(new WeekendsOnly());
        size_t holdingPeriodInBusinessDays = 20;
        PortfolioWithRegulatoryCapital::setMarginParameters(margin, calendar, holdingPeriodInBusinessDays);
    }
    
    void PortfolioB3CEM::setMarginParameters(boost::shared_ptr<Margin> margin, 
                                             boost::shared_ptr<Calendar> calendar, 
                                             size_t holdingPeriodInBusinessDays)
    {
        PortfolioWithRegulatoryCapital::setMarginParameters(margin, calendar, holdingPeriodInBusinessDays);
    }


    double PortfolioB3CEM::getRegulatoryCapital(Date valueDate)
    {
        if (errorTracking->getHasErrors()) 
        {
            numeric_limits<double>::quiet_NaN();
        }
        if (regCapAssetFactors == NULL)
        {
            return 0;
        }
        if (!creditRating->isOK())
        {
            return 0;
        }

        double mtm;
        double ngr = getNGR(valueDate, mtm);

        double EAD = 0;
        for (size_t i = 0; i < structures.size(); ++i) 
        {
            EAD += structures[i]->getMtMPlusAddOn(valueDate, ngr);
        }
        if (margin != NULL) 
        {
            EAD = EAD - mtm + max(0.0, mtm - margin->getMarginBalance());
        }
        else 
        {
            EAD = EAD - mtm + max(0.0, mtm);
        }
        double regCap = EAD * regCapCreditParameters->getCapitalCharge() * regCapCreditParameters->getCounterpartyWeighting();
        regCap = max(0.0, regCap);
        double M = getEffectiveMaturity(valueDate);
        // As per Basel III (http://www.bis.org/publ/bcbs189.pdf) pg 35, "For non-IMM banks the exposure (EAD) should be 
        // discounted by applying the factor (1-exp(-0.05*Mi))/(0.05*Mi)"
        regCap += 2.33 * creditRating->getRating() * M * EAD * (1-exp(-0.05*M)) / (0.05*M); // simplified in the event there are no hedges
        return regCap;
    }

    vector<double> PortfolioB3CEM::getRegulatoryCapitalAlongPath(const vector<double> spotValues)
    {
        if (errorTracking->getHasErrors()) 
        {
            return vector<double>(1,numeric_limits<double>::quiet_NaN());
        }

        if (regCapAssetFactors == NULL)
            return vector<double>(1,0);

        vector<double> regCap = vector<double>(spotValues.size());
        vector<double> portfolioMtM = vector<double>(spotValues.size());
        vector<double> marginBalance = vector<double>(spotValues.size());
    
        path = vector<double>(simulationSteps, 0);
        mtmOnPath = vector<double>(simulationSteps, 0);
        marginOnPath = vector<double>(simulationSteps, 0);

        exposurePathNoOffset = vector<double>(exposureSteps, 0);
        negativeExposurePathNoOffset = vector<double>(exposureSteps, 0);
      regCap = vector<double>(exposureSteps, 0);

        evolvePortfolioAlongPath(spotValues);
      collectStatsFromPath();

        for (size_t i = 0; i < regCap.size(); ++i) 
        {
            regCap[i] *= regCapCreditParameters->getCapitalCharge() * regCapCreditParameters->getCounterpartyWeighting();
            regCap[i] = max(0.0, regCap[i]);
        }

        //
        // Still to implment: CVA VAR!!!!!!!!!
        //

        return regCap;

    }

    double PortfolioB3CEM::getEffectiveMaturity(Date fromDate)
    {
        return numeric_limits<double>::quiet_NaN();
    }
    */
}

